# DATOS DE ENTRADA
data  = [
    (1000,10),
    (2000,13),
    (2500,170),
    (2500,-170),
]

# Imprime cabecera para referencia
print('INGRESOS | BENEFICIOS | PORCENTAJES')

#inicia la plantilla para alinear
TEMPLATE= "{ingresos:>7,}  | {beneficios:>+6}     | {porcentaje:>7.2%}"

#imprime los datos en filas
for revenue, profit in data:
    fila= TEMPLATE.format(ingresos=revenue, beneficios=profit, porcentaje=revenue/profit)
    print(fila)
    
