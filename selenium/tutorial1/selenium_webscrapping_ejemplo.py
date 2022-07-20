#Librerias
from selenium import webdriver
from selenium.webdriver.support.ui import WebDriverWait #Espera a que los elementos HTML carguen
from selenium.webdriver.support import expected_conditions as EC #bajo una condicion
from selenium.webdriver.common.by import By #por unos elementos de html
import time
import pandas as pd

#Opciones de navegación
options= webdriver.ChromeOptions()
options.add_argument('--start-maximized')
options.add_argument('--disable-extensions')

driver_path ='/home/eloy/Escritorio/TELECO/FSC/FSC2022/selenium/tutorial1/chromedriver'
driver=webdriver.Chrome(driver_path, chrome_options=options)

#Inicializamos el navegador
driver.get('https://rec.redsara.es/registro/action/are/acceso.do')

#vamos a darle 5 segundos a que cargue el HTML
WebDriverWait(driver, 5)\
    .until(EC.element_to_be_clickable((By.CSS_SELECTOR,
    'select#cdTipoTramite')))\
    .send_keys('r')

WebDriverWait(driver, 5)\
    .until(EC.element_to_be_clickable((By.XPATH,
    '/html/body/div[2]/form/fieldset/div/div/table/tbody/tr[3]/td[3]/div/span/span[2]')))\
    .click()
