.PHONY: hist, gaus, tgraph

hist: ## シンプルなヒストグラム
	root -l hist.C\(\"${run}\"\)

gaus: ## ガウスフィット
	root -l gaus.C\(\"${run}\"\)

tgraph: ## プロット
	root -l tgraph.C

gaus_compare: ## 9つ並べてガウスフィット、pdf保存
	root -l gaus_compare
