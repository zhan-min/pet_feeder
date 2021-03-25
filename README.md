一款宠物喂食器，基于stm32、rt-thread、Tuya SDK开发。<br>
This project is developed using Tuya SDK, which enables you to quickly develop branded apps connecting and controlling smart scenarios of many devices.
For more information, please check Tuya Developer Website.

一、方案标题
=
Pet_Feeder
=

二、应用场景
=
地点：养宠物的家庭<br>
功能：<br>
①自动喂食；可定制个性化喂食计划，设置喂食时间段、喂食量，实现自动按计划喂食。<br>
②状态数据实时上报；喂食记录、食物剩余量等数据实时记录并通过Wi-Fi通信模块上传。<br>
③远程喂食控制与监测；通过手机APP远程控制喂食，同时，食物剩余量、喂食情况等喂食器状态实时上报，手机可远程实时查看。<br>
④喂食器状态以及喂食提示音语音播报；通过语音模块实现数据语音播报，喂食时播放定制语音提醒宠物进食。<br>

三、开发方案
=
本项目开发的智能宠物喂食器主要包括：电源模块、主控模块、Wi-Fi通信和语音播报功能模块、电机驱动模块、本地控制模块、远程控制模块六大部分组成。<br>
电源模块：拟采用涂鸦三明治直流供电电源板和12V开关电源；<br>
主控模块：拟采用stm32f103芯片作为主控；<br>
Wi-Fi通信和语音播报功能模块：拟采用涂鸦三明治语音 Wi-Fi 通信板（VWXR2）；<br>
电机驱动模块：拟采用涂鸦三明治H桥直流电机驱动功能板，电机拟采用12V减速电机；<br>
本地控制模块：拟采用两个按键分别控制喂食和食物剩余及喂食记录等信息的语音播报；<br>
远程控制模块：拟采用手机APP配合涂鸦云端语音技能+IoT平台实现远程喂食控制、喂食记录查询等功能；<br>

四、开发计划
=
3月25前完成.<br>
1）3月5前准备物料<br>
2）3月1-20日嵌入式开发、云开发<br>
3）3月25日前整体调试。<br>
