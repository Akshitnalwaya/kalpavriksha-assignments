#include<stdio.h>
int main() {
    char s[100];
    gets(s);
    char t[100];
    int i=0,j=0,num=0;

    for(i=0,j=0;s[i]!='\0';i++) {
        if(!((s[i]=='+'||s[i]=='-'||s[i]=='*'||s[i]=='/') && (!i && s[i+1]!='\0' && (s[i+1]>='0' || s[i+1]<='9') && (i && s[i-1]>='0' || s[i-1]<='9')))) {
            printf("Error: Invalid Expression\n");
            return 0;
        }
        if(s[i]==' ')continue;
        if(s[i] >= '0' && s[i] <= '9') {
            num *= 10;
            num += s[i] - '0';
        }
        else if(s[i] == '+' || s[i] == '-') {
            char x[20];
            int y=0;
            while(num){
                x[y++]=num%10+'0';
                num/=10;
            }
            y--;
            while(y>=0) {
                t[j++] = x[y--];
            }
            t[j++] = s[i];
        }
        else {
            if(s[i] == '*') {
                int temp=0;
                i++;
                for(;s[i]!='\0' && !(s[i]=='+' || s[i]=='-' || s[i]=='*' || s[i]=='/');i++) {
                    if(s[i]==' ')continue;
                    temp *= 10;
                    temp += s[i] - '0';
                }
                num*=temp;
                i--;
            }
            else if(s[i] == '/') {
                int temp=0;
                i++;
                for(;s[i]!='\0' && !(s[i]=='+' || s[i]=='-' || s[i]=='*' || s[i]=='/');i++) {
                    if(s[i]==' ')continue;
                    temp *= 10;
                    temp += s[i] - '0';
                }
                if(!temp){ printf("Error: Division by zero\n"); return 0; }
                num/=temp;
                i--;
            }
        }
    }
    char x[20];
    int y=0;
    while(num){
        x[y++]=num%10+'0';
        num/=10;
    }
    y--;
    while(y>=0) {
        t[j++] = x[y--];
    }
    t[j] = '\0';

    int prev = 0 , curr = 0;
    int arr[20];
    j=0;
    for(i=0,j=0;t[i]!='\0';i++) {
        if(t[i] == '+' || t[i] == '-') {
            arr[j++] = curr;
            curr=0;
        }
        else {
            curr *= 10;
            curr += t[i] - '0';
        }
    }
    arr[j++] = curr;
    prev=arr[0];
    for(int i=0,j=1;t[i]!='\0';i++) {
        if(t[i] == '+') {
            prev+=arr[j++];
        }
        else if(t[i] == '-') prev -= arr[j++];
    }
    printf("Ans = %d", prev);
}