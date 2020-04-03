# connect6
Connect6. Strategy game similar to Gomoku. The one who gets six or more stones in a row first wins the game.

## Environment
* OS:   win7
* CPU:  intel i5-2400 3.10GHz
* RAM:  4GB
* IDE:  Code Blocks
* Compiler: mingw

## 棋盤結構
二維整數陣列19*19，沒有設置牆壁。
0 代表 空格  
1 代表 黑子  
2 代表 白子  
7 代表 標記  

** 棋盤介面參考: http://diiuuli520.blogspot.tw/2011/08/c-1p-vs-ai.html

## Evaluate
分數參考論文六子棋之棋型分類及審局函數之研究，目前只做 “活”棋型及”死”棋型 沒有”斷”棋型。會根據白方或黑方立場來算分數。採4個方向搜尋：水平、垂直、雙斜向。

以水平向為例，搜尋到棋串後，再判定其串左右兩方是否有敵方棋子，依此來判定為”活棋型”還是”死棋型”，然後根據棋串由幾顆子組成來給分。

若站黑方立場，則會算黑方棋型之後，在算白方棋型，兩者相減即為該盤面之分數。

在判斷其串左右方是否有敵方棋子時，也會判斷該棋串其中一邊是否在邊界，因此，一端黏著牆壁的連三，會判定成死三而不是活三。

## 範例 
此盤面若以黑子立場來看，可以看到有一組活三，和一組已經死掉的連四，故黑方有活三的分數1000。
白方有一組死掉的連四，然後沒有任何有分數的棋型，故0分。
因此此盤面在黑子立場來看，為+1000分。

## Alpha-beta pruning
因六子棋一次下兩子，若採用暴力展開，要展開361*361個盤面，速度實在太慢，
故我採用的方法為，在做alphabeta之前，先去為當下的盤面做上標記，以每顆盤面上的棋子往外延伸兩格，做上標記。

![example1]](images/example1.png)  
(請忽略周圍黑子)

如圖，若下了正中間黑子，則旁邊白子為標記的部分。

再做完標記之後，之後的alpha-beta pruning過程中，便只針對有標記的地方展開搜尋樹，如此一來可以大幅增加搜尋速度。

第二顆下子都會在第一顆下子之後，這可以再減少一半的時間。

速度: 搜索深度兩層，在簡單盤面可以迅速下子，複雜盤面思考約30秒內。
若深度加至三層，在簡單盤面就要思考一下子，複雜盤面要思考一陣子….


## 功能介紹
* 單人遊戲 (可選黑或白子)
* 雙人遊戲
* AI battle模式

