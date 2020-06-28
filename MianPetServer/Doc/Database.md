# userinfo
## 用于处理登录信息
|     id    |username|password|logintime|  online  |secretkey|heartbeat|
|:---------:|:------:|:------:|:-------:|:--------:|:-------:|:-------:|
|  CHAR(16) |CHAR(24)|CHAR(32)| DATETIME|TINYINT(1)| CHAR(20)| DATATIME|
|  NOT NULL |NOT NULL|NOT NULL| NOT NULL| NOT NULL |         |         |
|PRIMARY KEY|        |        |         |          |         |         |
### 注：在MySQL中用tinyint(1)代替boolean，MySQL并无真实的boolean类型
 
# petprofile
## 宠物实时数据
|     id    |username|      level      |    age     |   growth   |      food       |     clean       |     health     |      mood       |  growth_speed  |   status   | online_time|tuotuo|
|:---------:|:------:|:---------------:|:----------:|:----------:|:---------------:|:---------------:|:--------------:|:---------------:|:--------------:|:----------:|:----------:|:----:|
|  CHAR(16) |CHAR(24)|SMALLINT UNSIGNED|INT UNSIGNED|INT UNSIGNED|SMALLINT UNSIGNED|SMALLINT UNSIGNED|TINYINT UNSIGNED|SMALLINT UNSIGNED|TINYINT UNSIGNED|INT UNSIGNED|INT UNSIGNED| INT  |
|  NOT NULL |NOT NULL|     NOT NULL    |  NOT NULL  |  NOT NULL  |    NOT NULL     |    NOT NULL     |    NOT NULL    |     NOT NULL    |    NOT NULL    |  NOT NULL  |  NOT NULL  |      |
|PRIMARY KEY|        |                 |            |            |                 |                 |                |                 |                |            |            |      |
### 注：此处设计时间总和的单位均为分钟，客户端需注意转换单位

# shopinfo
## 商店货品信息
|  itemname |  price |description|      type      |
|:---------:|:------:|:---------:|:--------------:|
|  CHAR(18) |SMALLINT|VARCHAR(32)|TINYINT UNSIGNED|
|  NOT NULL |NOT NULL|           |    NOT NULL    |
|PRIMARY KEY|        |           |                |

# ownitems
## 用户拥有的物品
|  itemname |    quantity     |     type       |     id    |
|:---------:|:---------------:|:--------------:|:---------:|
|  CHAR(18) |SMALLINT UNSIGNED|TINYINT UNSIGNED|  CHAR(16) |
|  NOT NULL |    NOT NULL     |    NOT NULL    |  NOT NULL |
|PRIMARY KEY|                 |                |PRIMARY KEY|
