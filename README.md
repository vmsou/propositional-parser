# Construção de Interpretadores - Parser Lógica Proposicional

## Realizado
Aluno: Vinícius Marques da Silva de Oliveira <br>
Data: 31/08/2022

## Enunciado:
Para obter os pontos relativos a este trabalho, você deverá fazer um programa, usando a linguagem de programação que desejar, que seja capaz de validar expressões de lógica propisicional escritas em latex e definir se são expressões gramaticalmente corretas. Você validará apenas a forma da expressão (sintaxe).

A entrada será fornecida por um arquivo de textos que será carregado em linha de comando, com a seguinte formatação: 
1. Na primeira linha deste arquivo existe um número inteiro que informa quantas expressões lógicas estão no arquivo. 
2. Cada uma das linhas seguintes contém uma expressão lógica que deve ser validada.

A saída do seu programa será no terminal padrão do sistema e constituirá de uma linha de saída para cada expressão lógica de entrada contendo ou a palavra valida ou a palavra inválida e nada mais. 

### Gramática: 
1. Formula=Constante|Proposicao|FormulaUnaria|FormulaBinaria.
2. Constante="T"|"F".
3. Proposicao=[a−z0−9]+
4. FormulaUnaria=AbreParen OperadorUnario Formula FechaParen
5. FormulaBinaria=AbreParen OperatorBinario Formula Formula FechaParen
6. AbreParen="("
7. FechaParen=")"
8. OperatorUnario="¬"
9. OperatorBinario="∨"|"∧"|"→"|"↔"
 
Cada expressão lógica avaliada pode ter qualquer combinação das operações de negação, conjunção, disjunção, implicação e bi-implicação sem limites na combiação de preposições e operações. Os valores lógicos True e False estão representados na gramática e, como tal, podem ser usados em qualquer expressão de entrada. 

Para validar seu trabalho, você deve incluir no repl.it, no mínimo três arquivos contendo números diferentes de expressões proposicionais. O professor irá incluir um arquivo de testes extra para validar seu trabalho. Para isso, caberá ao professor incluir o arquivo no seu repl.it e rodar o seu programa carregando o arquivo de testes.

## Como executa-lo
Se você ter acesso a um terminal

```bash
mkdir build && cmake -S ./ -B ./build -G "MinGW Makefiles" && cmake --build ./build/

.\build\PropositionalParser.exe
```

## Status
<h4 align="center"> 
	✔️ Finalizado ✔️
</h4>
