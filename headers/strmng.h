#define STRMNG_DEFINED
#define NULLCHAR 0
#define LETTER 1
#define PUNCT 2
#define OPERATOR 2
#define GROUP 3
#define WHITE 4
#define QUOTE 5
#define NUMBER 6
#include <stdio.h>
#include <stdlib.h>
char *itostr(long int i, char s[])//converts i (integer) to a string
{
	long int sub = 10;
	int c = 0;
	int digit = 0;
	if(i<0){
		s[c++] = '-';
		i*=-1;
	}
	i+=1;
	while(i-sub > 0)
		sub*=10;
	sub/=10;
	while(sub > 0){
		while(i-sub > 0){
			i-=sub;
			digit++;
		}
		s[c++] = digit+'0';
		digit = 0;
		sub/=10;
	}
	s[c] = '\0';
	return s;

}

char *xtostr(long int i, unsigned int base, int lim, char s[]) {  // converts i (integer) to string in positive base 'base' (integer) where base is from 2 to 36, up to lim digits
	char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
	long int sub = base;
	int c = 0;
	int len = 1;
	int digit = 0;
	if(i<0){
		s[c++] = '-';
		i*=-1;
	}
	i+=1;
	while(i-sub > 0) {
		sub*=base;
		len++;
	}
	sub/=base;
	while((lim-len) > 0) {
		s[c++] = '0';
		len++;
	}
	while(sub > 0){
		while(i-sub > 0){
			i-=sub;
			digit++;
		}
		s[c++] = digits[digit];
		digit = 0;
		sub/=base;
	}
	s[c] = '\0';
	return s;
}

char *ptitostr(long int i, char *s)//converts i (integer) to a string
{
	s[100] = '\0';
	long int sub = 10;
	int c = 0;
	int digit = 0;
	if(i<0){
		s[c++] = '-';
		i*=-1;
	}
	i+=1;
	while(i-sub > 0)
		sub*=10;
	sub/=10;
	while(sub > 0){
		while(i-sub > 0){
			i-=sub;
			digit++;
		}
		s[c++] = digit+'0';
		digit = 0;
		sub/=10;
	}
	s[c] = '\0';
	return s;
}

void ftostr(double i, char s[]) //converts i (floating point) to a string
{
	long int sub = 10;
    long int pow = 1;
	int c = 0;
	int digit = 0;
	if(i<0){
		s[c++] = '-';
		i*=-1;
	}
	while(i-sub > 0)
		sub*=10;
	sub/=10;
	do{
		while(i-sub >= 0){
			i-=sub;
			digit++;
		}
		pow *= 10;
		i*=10;
		s[c++] = digit+'0';
		if(pow == sub*10 && i != 0)
			s[c++] = '.';
		digit = 0;
	}while(i > 0 || pow < sub*10);
	s[c] = '\0';

}

int isletter(char c) {
	return ((c >= 'a' && c <= 'z') || c >= 'A' || c <= 'Z') ? 1 : 0;
}

int isuppercase(char c) {
	return (c >= 'A' && c <= 'Z') ? 1 : 0;
}

int isnumber(char c) {
	return (c >= '0' && c <= '9') ? 1 : 0;
}

char *append(char str1[], char str2[]) //adds str2 to end of str1
{
	int i;
	for(i = 0; str1[i] != '\0'; i++)
		;
	for(int c = 0; str2[c] != '\0'; c++)
		str1[i++] = str2[c];
	str1[i] = '\0';
	return str1;
}

int iswhitespace(char s) // checks if it is whitespace
{
	return((s == ' ' || s == '\t' || s == '\n' || s == '\0'));
}

int len(char s[]) //length of s
{
    int i;
	for(i = 0; s[i] != '\0'; i++)
        ;
	return(i);
}

void prepend(char str1[], char str2[])  //adds str2 to beginning of str1
{
    int l2 = len(str2);
    int l1 = len(str1);
    for(int i = l1; i > -1; i--){
        str1[i+l2] = str1[i];
    }
    for(int i = 0; str2[i] != '\0'; i++)
        str1[i] = str2[i];
}

int chartype(char c) //returns complex character type
{
	if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return(LETTER);
	else if(c == '[' || c == ']' || c == '(' || c == ')' || c == '{' || c == '}')
		return(GROUP);
	else if(c == ' ' || c == '\n' || c == '\t')
		return(WHITE);
	else if(c == '\0')
		return(NULLCHAR);
	else if(c == '\'' || c == '\"')
		return(QUOTE);
	else if(c >= '0' && c <= '9')
		return(NUMBER);
	else
		return(PUNCT);
}

int simpletype(char c) //returns simple character type
{
	if(c == '\0')
		return(NULLCHAR);
	else if(c == ' ' || c == '\n' || c == '\t')
		return(WHITE);
	else
		return(LETTER);
}

int isword(char s1[], char s2[]) // checks if s1 and s2 are identical.
{
	int i;
	for(i = 0; s1[i] != '\0' && s2[i] != '\0' && s2[i] == s1[i]; i++)
		;
	return(s1[i] == '\0' && s2[i] == '\0');
}

int wordf(char str[], int wrd, char word[]) //finds character group number wrd (int), and puts it into word. not including whitespace. sorts by #define'd chartypes using (int) chartype. returns length ((int) wrdlen) of wrd'th word
{
	int w = 0;
	int wrdlen = 0;
	int c;
	char prevc = '\0';
	for(c = 0; str[c] != '\0' && w <= wrd; c++){
		if(w == wrd && chartype(str[c]) == WHITE)
			w++;
		if(chartype(prevc) != chartype(str[c]) && chartype(str[c]) != WHITE)
			w++;
		if(w == wrd)
			word[wrdlen++] = str[c];
		prevc = str[c];
	}
	word[wrdlen] = '\0';
	return(wrdlen);
}

int wordf2(char str[], int wrd, char word[]) //same as wordf but includes whitespace.
{
	int w = 0;
	char prevc = '\0';
	int c;
	int wrdlen = 0;
	for(c = 0; str[c] != '\0' && w <= wrd; c++){
		if(chartype(prevc) != chartype(str[c]))
			w++;
		if(w == wrd)
			word[wrdlen++] = str[c];
		prevc = str[c];

	}
	word[wrdlen] = '\0';
	return(wrdlen);

}

int simplewordf(char str[], int wrd, char word[]) //finds character group(word) (sorted by whitespace, null, or character) number wrd and puts it into word
{
	int w = 0;
	char prevc = '\0';
	int c;
	int wrdlen = 0;
	for(c = 0; str[c] != '\0' && w <= wrd; c++){
		if(simpletype(prevc) != simpletype(str[c]))
			w++;
		if(w == wrd)
			word[wrdlen++] = str[c];
		prevc = str[c];

	}
	word[wrdlen] = '\0';
	return(wrdlen);
}

void lower(char s[]) //converts all upper case letters in s to lower case
{
    for(int i = 0; s[i] != '\0'; i++){
        if(s[i] >= 'A' && s[i] <= 'Z')
            s[i] += 32;
    }

}

void upper(char s[]) //converts all lower case letters in s to upper case
{
    for(int i = 0; s[i] != '\0'; i++){
        if(s[i] >= 'a' && s[i] <= 'z')
            s[i] -= 32;
    }
}

int strindex(char s[], char word[])  //tells if word is in s
{
    int i, a;
    for(i = 0; s[i] != '\0'; i++){
        for(a = 0; s[a+i]!='\0' && s[a+i]==word[a]; a++)
            ;
        if(word[a] == '\0')
            return(1);
    }
    return(0);
}

int getl(char s[], int max)  //gets string from standard input
{
    int c, i;
    for(i = 0; i < max && (c = getchar()) != '\n'; i++)
        s[i] = c;
    s[i] = '\0';
    return((s[0] == '\0') ? 0 : 1);
}

void insertstr(char str[], int index, char word[]) // inserts word into str after index of str1
{
	int s1 = len(str);
	int s2 = len(word);
	for(int i = s1; i > index; i--){
		str[i+s2] = str[i];
	}
	for(int i = 0; i < s2; i++){
		str[i+index+1] = word[i];
	}
}

void insertchar(char str[], int index, char c) // inserts c into str after index of str
{
	int s1 = len(str);
	for(int i = s1; i > index; i--){
		str[i+1] = str[i];
	}
	str[index+1] = c;
}

void deletechar(char str[], int index) //deletes index of str
{
	for(int i = index; str[i] != '\0'; i++)
		str[i] = str[i+1];
}

void selects(char str[], int lowerlim, int upperlim, char selec[])
{
	int i;
	for(i = lowerlim; i < upperlim && str[i] != '\0'; i++)
		selec[i-lowerlim] = str[i];
	selec[i-lowerlim] = '\0';
}

int strend(char *s, char *t) // returns 1 if t is at the end of s
{
    while(*s != '\0'){
        for(int i = 0; t[i] == s[i]; i++){
            if(t[i] == '\0')
                return(1);
        }
        s++;
    }
    return(0);
}

void setchar (char *str1, char *str2, int max) 
{
	int i;
	for(i = 0; i < max && str2[i] != '\0'; i++)
		str1[i] = str2[i];
	str1[i] = '\0';
}