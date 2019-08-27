import sys
import re


def main():
    table_num = 1


    if(len(sys.argv) > 1):
        file = open(sys.argv[1], "r")
    else:
        file = sys.stdin


    html_code = file.read().replace('\n', '')

    table_block = re.findall(r'<table>(.*?)<\/table>', html_code, re.DOTALL|re.IGNORECASE)

    for x in range(1, len(table_block) + 1):
        print_table(x, table_block[x - 1])


def print_table(table_num, html_block):
    print("TABLE " + str(table_num) + ":")
    html_rows = re.findall(r'<\s*t\s*r\s*>(.*?)<\s*\/\s*t\s*r\s*>', html_block, re.DOTALL|re.IGNORECASE)

    print(','.join(re.findall(r'<\s*t\s*h\s*>(.*?)<\s*\/\s*t\s*h\s*>', html_rows[0])))

    for x in range(1, len(html_rows)):
        # print(re.findall(r'<td>(.*?)<\/td>',html_rows[x], re.IGNORECASE))
        # tmp_row = re.findall(r'<.*?>(.*?)<.*?>', html_rows[x],re.DOTALL|re.IGNORECASE)
        tmp_row = re.findall(r'<.*?>(.*?)?<.*?>', html_rows[x], re.DOTALL|re.IGNORECASE)
        csv_row = [x.strip(' ') for x in tmp_row]
        print(','.join(csv_row))

    print()


def remove_html_tags(data):
    p = re.compile(r'<.*?>')
    return p.sub('', data)


if __name__ == '__main__':
    main()