%{
	#include <stdio.h>
	#include "part3_helpers.hpp"
	#include "part3.tab.hpp"
	#include <string.h>
	#include <stdbool.h>

%}

%option yylineno
%option noyywrap


digit			[0-9]
id			[a-zA-Z]({digit}|[a-zA-Z]|_)*
integernum		{digit}+
realnum			{digit}+\.{digit}+
str			\"(([^"\n\r\\])|(\\[\"nt]))*\"
symbol			[(){},;:\.]
relop			==|<>|<|<=|>|>=
addop			\+|\-
mulop			\*|\/ 
assign			=
and			&&
or			\|\|
not			!
comment			#[^\n\r]*
whitespaces		[\t ]
newline			[\n\r]

%%

int				{
				yylval = new TokenNode(yytext);
				return INT;
				}
float			{
				yylval = new TokenNode(yytext);
				return FLOAT;
				}
void			{
				yylval = new TokenNode(yytext);
				return VOID;
				}
write			{
				yylval = new TokenNode(yytext);
				return WRITE;
				}
read			{
				yylval = new TokenNode(yytext);
				return READ;
				}
while			{
				yylval = new TokenNode(yytext);
				return WHILE;
				}
do				{
				yylval = new TokenNode(yytext);
				return DO;
				}
if				{
				yylval = new TokenNode(yytext);
				return IF;
				}
then			{
				yylval = new TokenNode(yytext);
				return THEN;
				}
else			{
				yylval = new TokenNode(yytext);
				return ELSE;
				}
return			{
				yylval = new TokenNode(yytext);
				return RETURN;
				}
full_while		{
				yylval = new TokenNode(yytext);
				return FULL_WHILE;
				}
break			{
				yylval = new TokenNode(yytext);
				return BREAK;
				}
{integernum}	{
				yylval = new TokenNode("integernum", yytext);
				return INTEGERNUM;
				}
{id}			{
				yylval = new TokenNode("id", yytext);
				return ID;
				}
{symbol}		{
				yylval = new TokenNode(yytext,);
				return yytext[0];
				}
{realnum}		{
				yylval = new TokenNode("realnum", yytext);
				return REALNUM;
				}
{str}			{
				char* s = yytext;
				s[yyleng]='\0';
				s = s+1;
				yylval = new TokenNode("str", s);
				return STR;
				}
{relop}			{
				yylval = new TokenNode("relop", yytext);
				return RELOP;
				}
{addop}			{
				yylval = new TokenNode("addop", yytext);
				return ADDOP;
				}
{mulop}			{
				yylval = new TokenNode("mulop", yytext);
				return MULOP;
				}
{assign}		{
				yylval = new TokenNode("assign", yytext);
				return ASSIGN;
				}
{and}			{
				yylval = new TokenNode("and", yytext);
				return AND;
				}
{or}			{
				yylval = new TokenNode("or", yytext);
				return OR;
				}
{not}			{
				yylval = new TokenNode("not", yytext);
				return NOT;
				}
{whitespaces}	{} 
{newline}		{}
{comment}		{}
.				{
				printf("Lexical error: '%s' in line number %d\n", yytext, yylineno);
				exit(1);
				}
%%

