# Primera prueba de scraping en python

from ast import Attribute
from urllib.request import urlopen
from urllib.error import HTTPError
from bs4 import BeautifulSoup

def getTitle(url):
    try:
        html=urlopen(url)
    except HTTPError as e:
        return None
    try:
        bs=BeautifulSoup(html.read(),'html.parser')
        title=bs.body.h1
    except AttributeError as e:
        return None
    return title


title=getTitle('http://pythonscraping.com/pages/page1.html')
if title==None:
    print('No se ha podido encontrar el titulo')
else:
    print(title)
