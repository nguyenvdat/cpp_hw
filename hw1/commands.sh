cat data/data.dat | wc -l # count number of lines
grep -c dolor data/data.dat # count lines contain a string
cat data/data.dat | wc -w # count words
grep -c -o '\mol\w*' data/data.dat # count words start with a string
ls data/test_folder | grep -c ".txt$" # count number of txt files