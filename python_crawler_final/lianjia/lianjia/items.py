# Define here the models for your scraped items
#
# See documentation in:
# https://docs.scrapy.org/en/latest/topics/items.html

import scrapy

class LianjiaItem(scrapy.Item):
    # define the fields for your item here like:
    # name = scrapy.Field()

    district = scrapy.Field()       # 区域
    city = scrapy.Field()           # 北上广深苏
    pattern = scrapy.Field()        # x居
    towards = scrapy.Field()        # 朝向
    title = scrapy.Field()          # 标题
    section = scrapy.Field()        # 板块
    area = scrapy.Field()           # 面积
    price = scrapy.Field()          # 月租
