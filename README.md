# SIGAP(Sistema Integrado de Gestão e Acompanhamento de Pacientes)
## Grupo 6
### 📘 Visão Geral
Nosso projto tem como proposito criar um sistema onde o usuario pode acompanhar seus registros referentes a sua diabetes, como consutas, exames, dieta e seus registros de glicose.

### 🏛️ Arquitetura do Classes
O projeto possui a seguinte estrutura de classes:
- Pessoa: Responsável por armazenar dados comuns a qualquer pessoa :Nome, CPF, Endereço, Sexo, Idade, Endereço e Senha;
- Paciente: Herda a classe *Pessoa* e armazena dados como : Tipo de diabetes, Tipos Sanguíneo, Altura, Peso, que são informações médicas importantes para o tratamento de um quadro de diabetes. Seus métodos são responsáveis pelo login no programa, agendamento e cancelamento de consultas e exames (em conjunto com as classes de registro), buscar id de um objeto paciente no banco de dados, impressão de registros clínicos;
- Medicação: Responsável por gerenciar as medicações do paciente, seus atributos são: Nome do Remédio, *Paciente*, Médico que receitou, Dosagem, Intervalo entre as administrações. Seus métodos são: Registrar no DB e Atualizar Dosagem
- Registro de Saúde: Classe responsável por ser base de todos os outros registros, seus atributos são: *Paciente*(faz referencia a um  objeto Paciente), Data e Hora. Seus métodos são: Registrar no DB e Exibir detalhes (estes são virtuais e são sobrepostos pelos registros);
- Registro de Consulta: Herda um *Registro de Saúde*, seus atributos são: Médico, Especialidade do Médico, Descrição (Uma breve descrição do motivo da consulta) e o Local. Seus métodos são: Registrar no DB e Exibir detalhes, que são uma sobreposição do métodos de *Registro de Saúde*;
- Registro de Exame: Herda um *Registro de Saúde*, seus atributos são: Nome do Exame, Médico Que Requisitou, Laboratório, Resultado (Um breve resumo do resultado do exame). Seus métodos são: Registrar no DB e Exibir detalhes, que são uma sobreposição do métodos de *Registro de Saúde*;
- Registro de Glicose: Herda um *Registro de Saúde*, seus atributos são: Nível de Glicose e um booleano de Jejum. Seus métodos são: Registrar no DB ( Este exibe um mensagem com um comentário sobre o nível de glicose) e Exibir detalhes, que são uma sobreposição do métodos de *Registro de Saúde*;
- Registro de Medicação: Herda um *Registro de Saúde*, seu atributo é: Uma referencia pra um *Medicamento* . Seus métodos são: Registrar no DB e Exibir detalhes, que são uma sobreposição do métodos de *Registro de Saúde*;
- Plano Alimentar: Classe responsável por gerenciar a dieta de um *Paciente* (Relação 1 para 1), seus atributos são: Nutricionista, *Paciente*, Carboidrato, Proteína, Gordura, Vitaminas e Alimentos Evitados. Seus métodos são: Registrar no DB e Atualizar o Plano Alimentar no DB;
- Métodos do Bancos de Dados: Classe responsável por fazer operações no banco de dados e outras operações mais genéricas de verificação, que não necessariamente dependem de um objeto de um tipo especifico, seus métodos são: Exibir Tabela de todos os *Registros* e Validar Entrada.

### 📊 Banco de Dados
O Banco de Dados foi a ferramenta usada no armazenamento de dados de objetos, foi usado o recurso de *Foreign Key* para Trabalhar com *Herança e Colaboração* dentro do Banco de Dados. A ferramenta usada foi o Sqlite3

### 🧍 Integrantes:
- Gabriel Costa Reis - 120549
- Marcos Vinícius Mariano Dias - 120560
- Victor Alexandre Siqueira Ribeiro - 120557
