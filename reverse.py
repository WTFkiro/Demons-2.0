from tqdm import tqdm
import os

def ReplaceLedsNumber(led):
    for i in range(0,len(led)-3):
        if led[i] == '[':
            if led[i+1] == 'i' or led[i+1] == 'j' or led[i+1] == 'k':
                pass
            elif led[i+2] == ']':
                placednum = str(ReshapeLeds(int(led[i+1])))
                newstr = led[:i+1]+placednum+led[i+2:]
                return newstr
            elif led[i+3] == ']':
                placednum = str(ReshapeLeds(int(led[i+1:i+3])))
                newstr = led[:i+1]+placednum+led[i+3:]
                return newstr
    return led

def ReshapeLeds(lednum):
    #如果该行为单数
    if lednum % 16 < 8:
    #改变编号
        lednum = int((lednum)/8)*8 + 7 - (lednum)%8
    return lednum

for file in os.listdir('Demons'):
    fread = open('Demons/'+file,"r",encoding='UTF-8') 
    lines = fread.readlines()
    fread.close()   
    count = len(lines) 
    pbar = tqdm(total = count)
    nowline = 0
    fwrite = open('C:/Users/15921/Desktop/DEMONS2/NEW/'+file, 'a',encoding='UTF-8')
    for line in lines:
        newline = ReplaceLedsNumber(line)
        fwrite.write(newline)
        nowline+=1
        pbar.update(1)  
        if nowline == count:
            pbar.close()
            print('\n'+file+' complete!')
            break