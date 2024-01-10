from matplotlib import ticker
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# 1. 比较 5 个城市的总体房租情况，包含租金的均价、最高价、最低价、中位数
# 等信息，单位面积租金（元/平米）的均价、最高价、最低价、中位数等信息。
# 采用合适的图或表形式进行展示。
def task1():
    # 计算单位面积租金和总租金的均价,最高价,最低价,中位数
    ave = df.groupby('city', as_index=True)['price'].mean()
    ave_per = df.groupby('city', as_index=True)['price-per-area'].mean()
    top = df.groupby('city', as_index=True)['price'].max()
    top_per = df.groupby('city', as_index=True)['price-per-area'].max()
    bot = df.groupby('city', as_index=True)['price'].min()
    bot_per = df.groupby('city', as_index=True)['price-per-area'].min()
    mid = df.groupby('city', as_index=True)['price'].median()
    mid_per = df.groupby('city', as_index=True)['price-per-area'].median()

    # 绘制单位面积租金和总租金的柱状图
    fig, ax1 = plt.subplots()
    ax1.set_title("总体房租情况")
    ax1.set_xlabel('城市')
    ax1.set_ylabel('总租金(元)', color='r')
    ax1.set_yscale('log')

    ax2 = ax1.twinx()
    ax2.set_ylabel('单位面积租金(元/平方米)', color='orange')
    ax2.set_yscale('log')

    x = np.arange(0, len(ave.index))
    plt.xticks(x, ave.index)

    # 柱(平均值)
    ax1.bar(x-0.1, ave, width=0.2, color='r')
    ax2.bar(x+0.1, ave_per, width=0.2, color='orange')
    for a, y in zip(x, ave):
        ax1.text(a-0.21, y, '平均%d' % y, ha='center', va='bottom', fontsize=8)
    for a, y in zip(x, ave_per.values):
        ax2.text(a+0.21, y, '平均%d' % y, ha='center', va='bottom', fontsize=8)

    # 点(最大,最小,中位数)
    ax1.scatter(x-0.1, top, color='black', s=6)
    ax1.scatter(x-0.1, bot, color='black', s=6)
    ax1.scatter(x-0.1, mid, color='black', s=6)
    for a, y in zip(x, top):
        ax1.text(a-0.2, y, '最大值%d' % y, ha='center', va='bottom', fontsize=8)
    for a, y in zip(x, bot):
        ax1.text(a-0.2, y, '最小值%d' % y, ha='center', va='bottom', fontsize=8)
    for a, y in zip(x, mid):
        ax1.text(a-0.2, y, '中位数%d' % y, ha='center', va='bottom', fontsize=8)

    ax2.scatter(x+0.1, top_per, color='black', s=6)
    ax2.scatter(x+0.1, bot_per, color='black', s=6)
    ax2.scatter(x+0.1, mid_per, color='black', s=6)
    for a, y in zip(x, top_per):
        ax2.text(a+0.2, y, '最大值%d' % y, ha='center', va='bottom', fontsize=8)
    for a, y in zip(x, bot_per):
        ax2.text(a+0.2, y, '最小值%d' % y, ha='center', va='bottom', fontsize=8)
    for a, y in zip(x, mid_per):
        ax2.text(a+0.2, y, '中位数%d' % y, ha='center', va='bottom', fontsize=8)
    plt.grid(True, linestyle='--', alpha=0.8)       # 网格线

    plt.show()

# 2. 比较 5 个城市一居、二居、三居的情况，包含均价、最高价、最低价、中位
# 数等信息。
def task2():
    # 计算一居,二居,三居的均价,最高价,最低价,中位数   
    ave = df[df['pattern'] != '四居+'].groupby([ 'city', 'pattern' ], as_index=True)['price'].mean()
    top = df[df['pattern'] != '四居+'].groupby([ 'city', 'pattern' ], as_index=True)['price'].max()
    bot = df[df['pattern'] != '四居+'].groupby([ 'city', 'pattern' ], as_index=True)['price'].min()
    mid = df[df['pattern'] != '四居+'].groupby([ 'city', 'pattern' ], as_index=True)['price'].median()

    fig, ax = plt.subplots()
    ax.set_title("一,二,三居房租情况")
    ax.set_xlabel('城市')
    ax.set_ylabel('租金(元)')
    ax.set_yscale('log')

    x = np.arange(0, len(ave.index.levels[0]))
    plt.xticks(x, ave.index.levels[0].values)
    
    # 柱(平均值)
    ax.bar(x-0.2, ave.xs('一居', level=1, drop_level=False), width=0.2, label='一居')
    ax.bar(x, ave.xs('二居', level=1, drop_level=False), width=0.2, label='二居')
    ax.bar(x+0.2, ave.xs('三居', level=1, drop_level=False), width=0.2, label='三居')
    plt.legend()
    for a, y in zip(x, ave.xs('一居', level=1, drop_level=False)):
        ax.text(a-0.4, y, '平均%d' % y, ha='center', va='bottom', fontsize=8)
    for a, y in zip(x, ave.xs('二居', level=1, drop_level=False)):
        ax.text(a, y, '平均%d' % y, ha='center', va='bottom', fontsize=8)
    for a, y in zip(x, ave.xs('三居', level=1, drop_level=False)):
        ax.text(a+0.4, y, '平均%d' % y, ha='center', va='bottom', fontsize=8)

    # 点(最大,最小,中位数)
    ax.scatter(x-0.2, top.xs('二居', level=1, drop_level=False), color='black', s=6)
    ax.scatter(x-0.2, bot.xs('二居', level=1, drop_level=False), color='black', s=6)
    ax.scatter(x-0.2, mid.xs('二居', level=1, drop_level=False), color='black', s=6)
    for a, y in zip(x, top.xs('二居', level=1, drop_level=False)):
        ax.text(a-0.3,y,'最大值%d' %y, ha='center', va='bottom', fontsize=8)
    for a, y in zip(x, bot.xs('二居', level=1, drop_level=False)):
        ax.text(a-0.3,y,'最小值%d' %y, ha='center', va='bottom', fontsize=8)
    for a, y in zip(x, mid.xs('二居', level=1, drop_level=False)):
        ax.text(a-0.3,y,'中位数%d' %y, ha='center', va='bottom', fontsize=8)
    
    ax.scatter(x, top.xs('二居', level=1, drop_level=False), color='black', s=6)
    ax.scatter(x, bot.xs('二居', level=1, drop_level=False), color='black', s=6)
    ax.scatter(x, mid.xs('二居', level=1, drop_level=False), color='black', s=6)
    for a, y in zip(x, top.xs('二居', level=1, drop_level=False)):
        ax.text(a,y,'最大值%d' %y, ha='center', va='top', fontsize=8)
    for a, y in zip(x, bot.xs('二居', level=1, drop_level=False)):
        ax.text(a,y,'最小值%d' %y, ha='center', va='top', fontsize=8)
    for a, y in zip(x, mid.xs('二居', level=1, drop_level=False)):
        ax.text(a,y,'中位数%d' %y, ha='center', va='top', fontsize=8)

    ax.scatter(x+0.2, top.xs('三居', level=1, drop_level=False), color='black', s=6)
    ax.scatter(x+0.2, bot.xs('三居', level=1, drop_level=False), color='black', s=6)
    ax.scatter(x+0.2, mid.xs('三居', level=1, drop_level=False), color='black', s=6)
    for a, y in zip(x, top.xs('三居', level=1, drop_level=False)):
        ax.text(a+0.3,y,'最大值%d' %y, ha='center', va='bottom', fontsize=8)
    for a, y in zip(x, bot.xs('三居', level=1, drop_level=False)):
        ax.text(a+0.3,y,'最小值%d' %y, ha='center', va='bottom', fontsize=8)
    for a, y in zip(x, mid.xs('三居', level=1, drop_level=False)):
        ax.text(a+0.3,y,'中位数%d' %y, ha='center', va='bottom', fontsize=8)

    plt.show()

# 3. 计算和分析每个城市不同板块的均价情况，并采用合适的图或表形式进行展示
def task3():
    # 计算每个城市不同板块的均价
    ave = df.groupby([ 'city', 'section' ], as_index=True)['price'].mean()

    # 方法:计算各板块的均价,根据均价统计处处于各价格区间内的板块的数量:
    # <=1500, 1501-2000, 2001-3000, 3001-5000, 5001-8000, 8001-20000, >20000
    xlabs = ['<=1500', '1501-2000', '2001-3000', '3001-5000', '5001-8000', '8001-20000', '>20000']
    fig, ax = plt.subplots()
    ax.set_title("各城市板块均价分布")
    ax.set_xlabel('价格区间')
    ax.set_ylabel('数量')
    x = np.arange(len(xlabs))
    plt.xticks(x, xlabs)
    ind = -2
    for city in ave.index.levels[0].values:
        a = ave[city]
        lvs = [0] * 7
        for v in a:
            if v <= 1500:
                lvs[0] += 1
            elif v <= 2000:
                lvs[1] += 1
            elif v <= 3000:
                lvs[2] += 1
            elif v <= 5000:
                lvs[3] += 1
            elif v <= 8000:
                lvs[4] += 1
            elif v <= 20000:
                lvs[5] += 1
            else:
                lvs[6] += 1
        
        ax.bar(x + ind * 0.15, lvs, width=0.15, label=city)
        ind += 1
        
    plt.legend()
    plt.show()

# 4. 比较各个城市不同朝向的单位面积租金分布情况，采用合适的图或表形式进
# 行展示。
def task4():
    # 计算每个城市不同朝向的单位面积租金的均值
    # ave_per = df.groupby([ 'city', 'towards' ], as_index=True)['price-per-area'].mean()
    ave_dong = df[df['towards'].str.contains("东", na=False)].groupby([ 'city' ], as_index=True)['price-per-area'].mean()
    ave_xi = df[df['towards'].str.contains("西", na=False)].groupby([ 'city' ], as_index=True)['price-per-area'].mean()
    ave_nan = df[df['towards'].str.contains("南", na=False)].groupby([ 'city' ], as_index=True)['price-per-area'].mean()
    ave_bei = df[df['towards'].str.contains("北", na=False)].groupby([ 'city' ], as_index=True)['price-per-area'].mean()

    fig, ax = plt.subplots()
    ax.set_title("各城市不同朝向均价分布")
    ax.set_xlabel('城市')
    ax.set_ylabel('均价(元/平方米)')
    x = np.arange(0, len(ave_dong.index))
    plt.xticks(x, ave_dong.index)

    ax.bar(x-0.3, ave_dong.values, width=0.2, label='东')
    ax.bar(x-0.1, ave_xi.values, width=0.2, label='西')
    ax.bar(x+0.1, ave_nan.values, width=0.2, label='南')
    ax.bar(x+0.3, ave_bei.values, width=0.2, label='北')
    for a, y in zip(x, ave_dong):
        ax.text(a-0.3, y, '%d' % y, ha='center', va='bottom', fontsize=8)
    for a, y in zip(x, ave_xi):
        ax.text(a-0.1, y, '%d' % y, ha='center', va='bottom', fontsize=8)
    for a, y in zip(x, ave_nan):
        ax.text(a+0.1, y, '%d' % y, ha='center', va='bottom', fontsize=8)
    for a, y in zip(x, ave_bei):
        ax.text(a+0.3, y, '%d' % y, ha='center', va='bottom', fontsize=8)

    plt.legend()
    plt.show()

plt.rcParams['font.sans-serif'] = ['Songti SC']
df = pd.read_csv("data.csv")

# 去重
df = df[['district', 'city', 'pattern', 'title', 'towards',
         'section', 'area', 'price']].drop_duplicates()

# 计算每个条目的单位面积租金
df['price-per-area'] = df['price'] / df['area']

# 剔除异常数据:
# 1. 面积超过1000平方米者
# 2. 单位面积租金小于一元者
tar = df[(df['price-per-area'] <= 1) | (df['area'] > 1000)].index
df.drop(tar, inplace=True)

task1()
# task2()
# task3()
# task4()
