import argparse
import configparser

from unittest import result

def main(numero, otro_numero):
    resultado= numero * otro_numero
    print(f'El resultado es {resultado}')
    
if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-n1', type=int, help='A number', default=1)
    parser.add_argument('-n2', type=int, help='Another number', default=1)
    parser.add_argument('--config', '-c', type=argparse.FileType('r'), help='config file')
    args = parser.parse_args()
    if args.config:
        config = configparser.ConfigParser()
        config.read_file(args.config)
        # Transforming values into integers
        args.n1 = int(config['ARGUMENTS']['n1'])
        args.n2 = int(config['ARGUMENTS']['n2'])

main(args.n1, args.n2)
