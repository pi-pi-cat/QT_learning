#!/bin/bash

# 获取当前激活的demo名称
DEMO_NAME=$(cat active_demo.conf | grep ACTIVE_DEMO | cut -d'=' -f2 | tr -d ' ')

# 检查demo名称是否为空
if [ -z "$DEMO_NAME" ]; then
    echo "Error: No active demo specified in active_demo.conf"
    exit 1
fi

# 检查应用程序是否存在
APP_PATH="build/bin/${DEMO_NAME}.app"
if [ ! -d "$APP_PATH" ]; then
    echo "Error: Demo application not found at $APP_PATH"
    exit 1
fi

# 运行应用程序
open "$APP_PATH" 