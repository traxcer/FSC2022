import argparse

def main(number, other_number):    
    result = number * other_number    
    print(f'El resultado es {result}')

if __name__ == '__main__':    
    parser = argparse.ArgumentParser()    
    parser.add_argument('-n1', type=int, help='Un numero', default=1)    
    parser.add_argument('-n2', type=int, help='Otro numero', default=1)    
    
    args = parser.parse_args()    
    
    main(args.n1, args.n2)