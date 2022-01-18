oDir="/tmp/bib"
install -d ${oDir}
tag=""
echo "git archive --format=tar --prefix=mg++${tag}/ HEAD | (cd /tmp/ && tar xf -)"
git archive --format=tar --prefix=mg++${tag}/ HEAD | (cd /tmp/ && tar xf -)
echo "cd /tmp/"
cd /tmp/
echo "tar czf ${oDir}/mg++${tag}.tgz mg++${tag}"
tar czf ${oDir}/mg++${tag}.tgz mg++${tag}
echo "cd /tmp/mg++${tag}/"
cd /tmp/mg++${tag}/
echo "git init"
git init
echo "find . | xargs git add >& /dev/null"
find . | xargs git add >& /dev/null
echo "sleep 1"
echo "git commit -m \"Initial commit\" >& /dev/null"
git commit -m "Initial commit" >& /dev/null
echo "cd .."
cd ..
echo "git clone --bare mg++${tag}"
git clone --bare mg++${tag}
echo "cd mg++${tag}.git/"
cd mg++${tag}.git/
echo "git remote remove origin"
git remote remove origin
echo "cd .."
cd ..
echo "tar czf ${oDir}/mg++${tag}_git.tgz mg++${tag}.git"
tar czf ${oDir}/mg++${tag}_git.tgz mg++${tag}.git
echo "rm -fr /tmp/mg++${tag}"
rm -fr /tmp/mg++${tag}
echo "rm -fr /tmp/mg++${tag}.git"
rm -fr /tmp/mg++${tag}.git
