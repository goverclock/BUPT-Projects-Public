# Define your item pipelines here
#
# Don't forget to add your pipeline to the ITEM_PIPELINES setting
# See: https://docs.scrapy.org/en/latest/topics/item-pipeline.html


# useful for handling different item types with a single interface
from itemadapter import ItemAdapter
import csv

class LianjiaPipeline:
    def open_spider(self, spider):
        try:
            self.file = open('data.csv', 'w', encoding="utf-8")
            self.firstline = True
        except Exception as err:
            print(err)
    
    def process_item(self, item, spider):
        dict_item = item.values()
        writer = csv.writer(self.file)
        if self.firstline:
            writer.writerow(item.keys())
            self.firstline = False
        writer.writerow(dict_item)
        return item

    def close_spider(self, spider):
        self.file.close()
