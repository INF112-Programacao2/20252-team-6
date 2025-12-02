# DiaryBetes
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

### 📝 User Stories

1. **Como paciente, eu quero criar uma conta no sistema** para poder acessar e gerenciar meus registros médicos relacionados à diabetes.
   - Critérios de aceitação: O sistema deve permitir cadastro com CPF, nome, endereço, senha e dados médicos (tipo de diabetes, tipo sanguíneo, altura, peso).

2. **Como paciente, eu quero fazer login no sistema** para acessar minhas informações pessoais e registros de saúde.
   - Critérios de aceitação: O sistema deve validar CPF e senha, e permitir acesso apenas com credenciais corretas.

3. **Como paciente, eu quero registrar meus níveis de glicose** para acompanhar minha condição ao longo do tempo.
   - Critérios de aceitação: O sistema deve permitir registrar data, hora, nível de glicose e se estava em jejum, além de exibir comentários sobre o nível registrado.

4. **Como paciente, eu quero visualizar o histórico de glicose em gráfico ou tabela** para entender melhor a evolução dos meus níveis de açúcar no sangue.
   - Critérios de aceitação: O sistema deve exibir registros ordenados por data e hora, permitindo visualização em formato de tabela ou gráfico.

5. **Como paciente, eu quero registrar consultas médicas e exames** para manter um histórico completo do meu tratamento.
   - Critérios de aceitação: O sistema deve permitir registrar consultas (médico, especialidade, descrição, local) e exames (nome, laboratório, resultado, médico requisitante) com data e hora.

6. **Como paciente, eu quero gerenciar meu plano alimentar e medicações** para acompanhar minha dieta e tratamento medicamentoso.
   - Critérios de aceitação: O sistema deve permitir criar e atualizar plano alimentar (nutricionista, carboidratos, proteínas, gorduras, vitaminas, alimentos evitados) e registrar uso de medicações com dosagem e intervalo.

### 🚀 Instalação e Execução

#### Pré-requisitos
- Sistema operacional: Linux (Ubuntu/Debian) ou Windows
- Compilador: g++ (GCC) com suporte a C++17
- Make: ferramenta para build
- SQLite3: banco de dados

#### Instalação no Linux (Ubuntu/Debian)

1. **Clone o repositório:**
```bash
git clone https://github.com/seu-usuario/20252-team-6.git
cd 20252-team-6
```

2. **Instale as dependências:**
```bash
sudo apt-get update
sudo apt-get install -y build-essential g++ make sqlite3 libsqlite3-dev
```

3. **Compile o projeto:**
```bash
make
```

4. **Execute o programa:**
```bash
./diarybetes
```

#### Instalação no Windows

1. **Clone o repositório:**
```bash
git clone https://github.com/seu-usuario/20252-team-6.git
cd 20252-team-6
```

2. **Instale as dependências:**
   - **MinGW-w64 ou MSYS2:**
     - Baixe e instale o MinGW-w64 ou MSYS2
     - Adicione o caminho do g++ ao PATH do sistema
   
   - **Make:**
     - Instale via Chocolatey: `choco install make`
     - Ou baixe de: https://www.gnu.org/software/make/
   
   - **SQLite3:**
     - Baixe de: https://www.sqlite.org/download.html
     - Extraia e adicione ao PATH, ou coloque os arquivos na pasta do projeto

3. **Compile o projeto:**
```bash
make
```

4. **Execute o programa:**
```bash
diarybetes.exe
```

#### Comandos Úteis

- `make` - Compila o projeto
- `make clean` - Remove arquivos compilados (.o e executável)
- `make rebuild` - Limpa e recompila tudo

#### Estrutura do Projeto
```
20252-team-6/
├── include/          # Headers (.hpp)
├── src/              # Código fonte (.cpp)
├── main.cpp          # Arquivo principal
├── Makefile          # Arquivo de build
└── README.md         # Este arquivo
```

### 🛠️ Outras Ferramentas
Foi usado:
- Notion e Slack para gerenciamento;
- Discord para reuniões;
- GitHub para versionmeto de código

### 🧍 Integrantes:
- Gabriel Costa Reis - 120549
- Marcos Vinícius Mariano Dias - 120560
- Victor Alexandre Siqueira Ribeiro - 120557
