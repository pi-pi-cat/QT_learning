TEMPLATE = subdirs

# 控制要编译的子项目，取消注释相应的行即可编译对应的demo
SUBDIRS += \
    MetaObjectDemo \


# 默认的构建和部署规则
CONFIG += ordered

# 创建bin目录用于存放所有demo的可执行文件
system(mkdir -p $$OUT_PWD/bin)

# 通用的构建设置
CONFIG += debug_and_release
CONFIG += build_all

# 确保清理时也清理子目录
QMAKE_CLEAN += $$OUT_PWD/bin/* 