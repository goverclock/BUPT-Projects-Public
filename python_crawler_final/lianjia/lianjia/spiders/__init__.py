# This package will contain the spiders of your Scrapy project
#
# Please refer to the documentation for information on how to create and manage
# your spiders.

import logging
import scrapy
from lianjia.items import LianjiaItem

class LianjiaSpider(scrapy.spiders.Spider):
    name = 'lianjia'

    item_got = 0
    page_got = 0
    logging.getLogger('scrapy').propagate = False

    cities = ['bj', 'sh', 'gz', 'sz', 'su']  # 北京, 上海, 广州, 深圳, 苏州
    patterns = ['l0', 'l1', 'l2', 'l3']      # 一居, 二居, 三居, 四居+

    start_urls = []
    for c in cities:
        start_urls.append("https://"+c+".lianjia.com/zufang/")

    # 第一级parse,获取一个城市的地区列表和价格区间数量
    # 输入: 例如 北京 https://bj.lianjia.com/zufang/
    def parse(self, response):
        district = []
        district_path = "/html/body/div[3]/div[1]/div[4]/div[1]/ul[2]/*"
        for district_ul in response.xpath(district_path):
            text = district_ul.xpath(".//a/text()").extract()[0]
            pinyin = district_ul.xpath(".//a/@href").extract()[0][8:-1]
            if text == "不限":
                continue
            district.append([text, pinyin])

        priceRange = []
        pr_path = '//*[@id="filter"]/ul[6]/*'
        pr_cnt = len(response.xpath(pr_path)) - 2
        for i in range(1, pr_cnt + 1):
            priceRange.append('rp' + str(i))

        # 根据 地区,价格区间,户型 三个条件筛选后的URL分别爬取
        for d in district:
            for pr in priceRange:
                for p in self.patterns:
                    # https://bj.lianjia.com/zufang/chaoyang/l1rp7/
                    url = response.request.url + d[1] + "/" + p + pr
                    yield scrapy.Request(
                        url,
                        callback=self.parse2,
                        meta={
                            'city': response.request.url[8:10],
                            'district': d[0],
                            'pattern': p,
                            'pr': pr
                        }
                    )

    # 第二级parse,在已经确定了地区和价格区间的页面中获取租房信息页数,并对每一页进行爬取
    # 输入: 例如 北京(bj) 朝阳(chaoyang) 两居(l1) >=20000元(rp7) https://bj.lianjia.com/zufang/chaoyang/l1rp7/
    def parse2(self, response):
        # ti = response.css('span.content__title--hl').xpath('.//text()').extract()[0]
        # self.item_got += int(ti)
        # self.page_got += 1
        # print(self.page_got, ti, self.item_got)
        # with open('fuck.txt', 'a') as f:
        #     f.writelines(f"{response.request.url}, {self.page_got}, {ti}, {self.item_got}\n")
        # return
        
        # 获取此筛选条件下的总页数
        cp = response.css('div.content__pg').xpath(".//@data-totalpage")
        tot_page = 0
        if len(cp):
            tot_page = int(cp.extract()[0])

        # 如果页数为0,说明没有租房信息
        if tot_page > 0:
            for i in range(1, tot_page + 1):
                url = response.request.url
                x = response.request.url.rfind('/') + 1
                # https://bj.lianjia.com/zufang/xicheng/pg2l1rp6/
                url = url[:x] + 'pg' + str(i) + url[x:]
                yield scrapy.Request(
                    url,
                    callback=self.parse3,
                    meta=response.meta
                )

    # 第三级parse,在已经确定了筛选条件和页面的界面中获取实际的租房信息
    def parse3(self, response):
        city_dict = {
            'bj': '北京',
            'sh': '上海',
            'gz': '广州',
            'sz': '深圳',
            'su': '苏州'
        }
        pattern_dict = {
            'l0': '一居',
            'l1': '二居',
            'l2': '三居',
            'l3': '四居+'
        }
        item = LianjiaItem()
        item['district'] = response.meta.get('district')
        item['city'] = city_dict[response.meta.get('city')]
        item['pattern'] = pattern_dict[response.meta.get('pattern')]

        content_list = response.xpath(
            "/html/body/div[3]/div[1]/div[5]/div[1]/div[1]/*")

        found = 0
        for content_item in content_list:       # 处理每条租房信息
            found += 1

            tit = content_item.xpath(".//a/@title").extract()[0]   # 标题
            desc = content_item.xpath(
                ".//div/p[2]/a[2]/text()").extract()  # 板块
            sec = ''
            if desc != []:
                sec = desc[0]
            area = -1
            t = content_item.css('p.content__list--item--des').xpath('.//text()').extract()
            for s in t:
                if '㎡' in s:
                    s = s.replace(" ", '').strip('\n').strip('㎡')
                    if '-' in s:        # 一些租房信息使用形如20.00㎡-30.00㎡的方式表示面积
                        x = s.rfind('-')    # 这种情况取区间的中值作为代表
                        lv = float(s[0:x])
                        rv = float(s[x + 1:])
                        area = (lv + rv) / 2
                    else:
                        area = s
                    
            t = content_item.xpath(
                ".//div/span/em/text()").extract()[0]   # 价格
            if '-' in t:            # 同面积的情况
                x = t.rfind('-')
                lv = float(t[0:x])
                rv = float(t[x + 1:])
                price = (lv + rv) / 2
            else:
                price = t

            item['title'] = tit
            to = ''
            for dir in ['东', '西', '南', '北']:
                if dir in tit:
                    to += dir
            item['towards'] = to
            item['section'] = sec
            item['area'] = area
            item['price'] = price
            yield (item)

        self.item_got += found
        self.page_got += 1
        print("page crawled:", self.page_got, "items(current page):", found, "items(total)",self.item_got)
