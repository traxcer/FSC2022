# Primera prueba de scraping en python

from urllib.request import urlopen
from bs4 import BeautifulSoup

html=urlopen('http://pythonscraping.com/pages/page1.html')
print ('El tipo de html es:%s',type(html))
bs = BeautifulSoup(html,'html.parser')
print(bs.h1)
