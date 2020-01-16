CppParser
=========

## Método de uso:

    python CppParser.py <pastaDosProjetos> <pastaTemplates>

<pastaDosProjetos> é uma pasta contendo varias outras pastas, uma por cada projeto. O software não faz varredura recursiva, só pega as pastas do primeiro nível. Dentro de <pastaDosProjetos> vai ser gerado o arquivo output.csv contendo o resultado separado por vírgulas.

A pasta <pastaTemplates> tem de conter um arquivo .template por cada classe válida para o EP. Este arquivo é uma simplificação dum arquivo .h, com a seguinte forma:

    exemplo.template
    ============================================================================
    class Exemplo

    public:
    atribute1
    atribute2
    method1()
    method2()

    protected:
    ALL
    ALL()

    private:
    ALL
    ALL()
    ============================================================================

Para os arquivos template não são necessários (mas se usados, serão ignorados):
    + Guardas de código
    + parâmetros dos métodos
    + tipos de dados de atributos e métodos
    + ponto e virgula
    + indentação

É importante notar que se você deseja permitir qualquer atributo ou método de um modificador de visualização específico, usa-se "ALL" para atributos e "ALL()" para métodos.

## Arquivo de saida: Output.csv

O formato é um arquivo de saída é csv (separado por vírgulas mesmo). As calunas do arquivo são:

+ NUSP: Nome da pasta
+ CamelCase: quantidade de membros (atributos e métodos) com nome fora da convenção lowerCamelCase (só verifica 1o caractere).
+ Extra Methods: quantidade de métodos públicos/protegidos/privados além dos especificados no enunciado.
+ Extra Attributes: quantidade de atributos públicos/protegidos/privados além dos especificados no enunciado.
+ Unused Private Parameter: quantidade de atributos não usados.
+ Extra Classes: quantidade de classes não especificadas no enunciado.
+ Bad Practices: quantidade de más práticas de programação identificadas pelo CppCheck.
+ Duplicated Lines: quantidade de código duplicado (9 linhas ou mais) identificado pelo Simian.


