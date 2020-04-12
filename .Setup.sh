alias m='make | less'
alias e='ls *cpp *h | entr -c make -s'

g(){
	git add -A
	testStr="$@"
	git commit -m "$testStr"
}


