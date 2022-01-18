
for i in $(git ls-tree -r --name-only HEAD | egrep -v "^obj" | egrep -v "^libjpeg" | egrep "\.[ch]$"); do
	echo $i;
	emacs -batch $i --eval '(setq tab-width 4)' --eval '(setq c-basic-offset 4)' --eval '(indent-region (point-min) (point-max) nil)' --eval '(whitespace-cleanup)' -f save-buffer
done
