# VRP_project
 Vehicle Route Problem

此演算法是針對物流的路徑規劃，演算法最終目標是決定要用幾台貨車送貨，並且每台車的送貨路徑為何。

困難的地方有以下幾點:
1. 每個店家會提供時間區間，要完成送貨就需要在時間區間內抵達
2. 所有貨車需要在規定時間內返回倉庫
3. 貨車有載重限制

即使符合所有限制，還需要另外判斷"解"的好壞。比方說用了10台車且總路徑800公里跟用了9台車且總路徑950公里這兩個解到底誰好壞。

![image](https://github.com/user-attachments/assets/278029cf-6a95-4a41-8392-7573cf646021)
