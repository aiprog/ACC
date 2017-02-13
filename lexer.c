/*  c.l
 *
 *  ANSI C grammar, Lex specification
 */

D     [0-9]        
L     [a-zA-Z_]
O     [0-7]
H     [a-fA-F0-9]
I     (u|U|l|L)*

%{

#include <stdio.h>
  
#include "y.tab.h"
#include "common.h"
#include "gen.h"

extern int fileno (FILE *stream);

#define POS_CPY yylval.id.position = position;

%}

%%

"break"     { errorUpdatePosition (); POS_CPY return (BREAK); }
"char"      { errorUpdatePosition (); POS_CPY return (CHAR); }
"const"     { errorUpdatePosition (); POS_CPY return (CONST); }
"continue"  { errorUpdatePosition (); POS_CPY return (CONTINUE); }
"do"        { errorUpdatePosition (); POS_CPY return (DO); }
"else"      { errorUpdatePosition (); POS_CPY return (ELSE); }
"extern"    { errorUpdatePosition (); POS_CPY return (EXTERN); }
"for"       { errorUpdatePosition (); POS_CPY return (FOR); }
"if"        { errorUpdatePosition (); POS_CPY return (IF); }
"int"       { errorUpdatePosition (); POS_CPY return (INT); }
"return"    { errorUpdatePosition (); POS_CPY return (RETURN); }
"short"     { errorUpdatePosition (); POS_CPY return (SHORT); }
"signed"    { errorUpdatePosition (); POS_CPY return (SIGNED); }
"sizeof"    { errorUpdatePosition (); POS_CPY return (SIZEOF); }
"static"    { errorUpdatePosition (); POS_CPY return (STATIC); }
"struct"    { errorUpdatePosition (); POS_CPY return (STRUCT); }
"unsigned"  { errorUpdatePosition (); POS_CPY return (UNSIGNED); }
"void"      { errorUpdatePosition (); POS_CPY return (VOID); }
"volatile"  { errorUpdatePosition (); POS_CPY return (VOLATILE); }
"while"     { errorUpdatePosition (); POS_CPY return (WHILE); }

{L}({L}|{D})*          { 
                         errorUpdatePosition (); 
                         yylval.id.lexeme = strdup (yytext); 
                         POS_CPY return (IDENTIFIER); 
                       }

{D}+                   { 
                         errorUpdatePosition (); 
                         sscanf (yytext, "%d", &yylval.id.value);
                         POS_CPY return (CONSTANT); 
                       }

'[^']'                 { 
                         errorUpdatePosition ();
                         yylval.id.value = yytext[1];
                         POS_CPY return (CONSTANT); 
                       }
\"(\\.|[^\\\"])*\"     { 
                         errorUpdatePosition ();
                         yylval.id.lexeme = strdup (yytext);
                         POS_CPY return (STRING_LITERAL); 
                       }

"..."         { errorUpdatePosition (); POS_CPY return (ELLIPSIS); }
"->"          { errorUpdatePosition (); POS_CPY return (PTR_OP); }
"&&"          { errorUpdatePosition (); POS_CPY return (AND_OP); }
"||"          { errorUpdatePosition (); POS_CPY return (OR_OP); }
"<="          { errorUpdatePosition (); POS_CPY return (LE_OP); }
">="          { errorUpdatePosition (); POS_CPY return (GE_OP); }
"=="          { errorUpdatePosition (); POS_CPY return (EQ_OP); }
"!="          { errorUpdatePosition (); POS_CPY return (NE_OP); }
";"           { errorUpdatePosition (); POS_CPY return (';'); }
"{"           { errorUpdatePosition (); POS_CPY return ('{'); }
"}"           { errorUpdatePosition (); POS_CPY return ('}'); }
","           { errorUpdatePosition (); POS_CPY return (','); }
":"           { errorUpdatePosition (); POS_CPY return (':'); }
"="           { errorUpdatePosition (); POS_CPY return ('='); }
"("           { errorUpdatePosition (); POS_CPY return ('('); }
")"           { errorUpdatePosition (); POS_CPY return (')'); }
"["           { errorUpdatePosition (); POS_CPY return ('['); }
"]"           { errorUpdatePosition (); POS_CPY return (']'); }
"."           { errorUpdatePosition (); POS_CPY return ('.'); }
"&"           { errorUpdatePosition (); POS_CPY return ('&'); }
"!"           { errorUpdatePosition (); POS_CPY return ('!'); }
"-"           { errorUpdatePosition (); POS_CPY return ('-'); }
"+"           { errorUpdatePosition (); POS_CPY return ('+'); }
"*"           { errorUpdatePosition (); POS_CPY return ('*'); }
"/"           { errorUpdatePosition (); POS_CPY return ('/'); }
"%"           { errorUpdatePosition (); POS_CPY return ('%'); }
"<"           { errorUpdatePosition (); POS_CPY return ('<'); }
">"           { errorUpdatePosition (); POS_CPY return ('>'); }

"/*"(\*[^\/]|[^\*])*"*/" { errorUpdatePosition (); }
[ \t\v\n\f]              { errorUpdatePosition (); }    
.                           

%%

int yywrap ()
{
  (void) yyunput;
  (void) input;
  
  return 1;
}