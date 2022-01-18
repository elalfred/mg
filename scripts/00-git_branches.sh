#!/bin/bash


echo "git checkout 2013_noshading"
echo "git rebase master"
echo "git checkout 2013_nocamera"
echo "git rebase 2013_noshading"
echo "git checkout 2013_noscene"
echo "git rebase 2013_nocamera"
