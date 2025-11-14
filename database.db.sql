BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "Medicacao" (
	"Id"	INTEGER,
	"Paciente"	INTEGER,
	"Nome"	TEXT,
	"Horario"	TEXT,
	"Dosagem"	REAL,
	"Medico"	TEXT,
	PRIMARY KEY("Id" AUTOINCREMENT),
	FOREIGN KEY("Paciente") REFERENCES "Paciente"("Id")
);
CREATE TABLE IF NOT EXISTS "Paciente" (
	"Id"	INTEGER,
	"Pessoa"	INTEGER,
	PRIMARY KEY("Id" AUTOINCREMENT),
	FOREIGN KEY("Pessoa") REFERENCES "Pessoa"("id")
);
CREATE TABLE IF NOT EXISTS "Pessoa" (
	"id"	INTEGER,
	"Nome"	TEXT,
	"Cpf"	TEXT,
	"Sexo"	TEXT,
	"Senha"	INTEGER,
	"Endereco"	TEXT,
	PRIMARY KEY("id" AUTOINCREMENT)
);
CREATE TABLE IF NOT EXISTS "PlanoAlimentar" (
	"Id"	INTEGER,
	"Paciente"	INTEGER,
	"AlimentosEvitados"	TEXT,
	"Proteinas"	INTEGER,
	"Carboidrato"	INTEGER,
	"Gordura"	INTEGER,
	"Vitaminas"	TEXT,
	"Nutricionista"	TEXT,
	PRIMARY KEY("Id" AUTOINCREMENT),
	FOREIGN KEY("Paciente") REFERENCES "Paciente"("Id")
);
CREATE TABLE IF NOT EXISTS "RegistroConsulta" (
	"Id"	INTEGER,
	"RegistroSaude"	INTEGER,
	"Medico"	TEXT,
	"Descricao"	TEXT,
	"Local"	TEXT,
	PRIMARY KEY("Id" AUTOINCREMENT),
	FOREIGN KEY("RegistroSaude") REFERENCES "RegistroSaude"("Id")
);
CREATE TABLE IF NOT EXISTS "RegistroExame" (
	"Id"	INTEGER,
	"RegistroSaude"	INTEGER,
	"Nome"	TEXT,
	"Medico"	TEXT,
	"Resultado"	TEXT,
	PRIMARY KEY("Id" AUTOINCREMENT),
	FOREIGN KEY("RegistroSaude") REFERENCES "RegistroSaude"("Id")
);
CREATE TABLE IF NOT EXISTS "RegistroGlicose" (
	"Id"	INTEGER,
	"RegistroSaude"	INTEGER,
	"NivelGlicose"	REAL,
	PRIMARY KEY("Id" AUTOINCREMENT),
	FOREIGN KEY("RegistroSaude") REFERENCES "RegistroSaude"("Id")
);
CREATE TABLE IF NOT EXISTS "RegistroMedicacao" (
	"Id"	INTEGER,
	"RegistroSaude"	INTEGER,
	"Medicacao"	INTEGER,
	PRIMARY KEY("Id" AUTOINCREMENT),
	FOREIGN KEY("Medicacao") REFERENCES "Medicacao"("Id"),
	FOREIGN KEY("RegistroSaude") REFERENCES "RegistroSaude"("Id")
);
CREATE TABLE IF NOT EXISTS "RegistroSaude" (
	"Id"	INTEGER,
	"Paciente"	INTEGER,
	"Hora"	TEXT,
	"Data"	TEXT,
	PRIMARY KEY("Id" AUTOINCREMENT),
	FOREIGN KEY("Paciente") REFERENCES "Paciente"("Id")
);
COMMIT;