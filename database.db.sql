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
	"TipoSanguineo"	TEXT,
	"TipoDiabetes"	TEXT,
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
	"Especialidade"	TEXT,
	PRIMARY KEY("Id" AUTOINCREMENT),
	FOREIGN KEY("RegistroSaude") REFERENCES "RegistroSaude"("Id")
);
CREATE TABLE IF NOT EXISTS "RegistroExame" (
	"Id"	INTEGER,
	"RegistroSaude"	INTEGER,
	"Nome"	TEXT,
	"Medico"	TEXT,
	"Resultado"	TEXT,
	"Laboratorio"	TEXT,
	PRIMARY KEY("Id" AUTOINCREMENT),
	FOREIGN KEY("RegistroSaude") REFERENCES "RegistroSaude"("Id")
);
CREATE TABLE IF NOT EXISTS "RegistroGlicose" (
	"Id"	INTEGER,
	"RegistroSaude"	INTEGER,
	"NivelGlicose"	REAL,
	"Jejum"	BLOB,
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
INSERT INTO "Paciente" VALUES (1,1,NULL,NULL);
INSERT INTO "Paciente" VALUES (2,3,NULL,NULL);
INSERT INTO "Pessoa" VALUES (1,'Pedro','12345678901',NULL,NULL,NULL);
INSERT INTO "Pessoa" VALUES (2,'Pedro','12345678901',NULL,NULL,NULL);
INSERT INTO "Pessoa" VALUES (3,'Pedro Alves','12345678902',NULL,NULL,NULL);
INSERT INTO "PlanoAlimentar" VALUES (1,1,' ',140,200,20,'B12, A','Jessica');
INSERT INTO "PlanoAlimentar" VALUES (2,1,'Refrigerante, chocolate',120,40,50,'B12, A','Lucas');
INSERT INTO "PlanoAlimentar" VALUES (3,1,'Refrigerante, chocolate',120,200,20,'B12, A','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (4,1,'Refrigerante, chocolate',120,200,20,'B12, A','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (5,1,'Refrigerante, chocolate',120,200,20,'B12, A','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (6,1,'Refrigerante, chocolate',120,200,20,'B12, A','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (7,1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (8,1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (9,1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (10,1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (11,1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (12,1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (13,1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (14,1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (15,1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (16,1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (17,1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (18,1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (19,1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (20,1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (21,1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (22,1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (23,1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (24,1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (25,1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (26,2,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (27,2,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (28,2,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (29,2,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (30,2,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (31,2,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (32,2,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (33,2,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (34,2,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (35,2,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (36,-1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (37,-1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (38,-1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (39,-1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (40,-1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (41,-1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (42,-1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (43,-1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (44,-1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (45,-1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (46,-1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (47,-1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (48,-1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (49,-1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (50,-1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (51,-1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (52,-1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
INSERT INTO "PlanoAlimentar" VALUES (53,-1,'Refrigerante, chocolate',120,200,20,'B12, C','Sandra');
COMMIT;
