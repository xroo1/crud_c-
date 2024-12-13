#include <iostream>
#include <sqlite3.h>
#include <string>

using namespace std;

int callback(void* data, int argc, char** argv, char** colNames) {
    for (int i = 0; i < argc; i++) {
        std::cout << colNames[i] << ": " << (argv[i] ? argv[i] : "NULL") << std::endl;
    }
    std::cout << std::endl;
    return 0;
}

int connectDB(sqlite3*& db) {
    int exit = sqlite3_open("database.db", &db);

    if (exit) {
        cerr << "Erro ao abrir o banco de dados: " << sqlite3_errmsg(db) << endl;
        return -1;
    } else {
        cout << "Banco de dados conectado com sucesso" << endl;
    }
    return 0;
}

void creatDB(sqlite3* db) {
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS cliente (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT,
            year TEXT,
            city TEXT
        );
    )";

    char* mensagemErro;
    int exit = sqlite3_exec(db, sql, nullptr, nullptr, &mensagemErro);

    if (exit != SQLITE_OK) {
        cerr << "Erro ao criar tabela: " << mensagemErro << endl;
        sqlite3_free(mensagemErro);
    } else {
        cout << "Tabela criada com sucesso!" << endl;
    }
}

void inserirDB(sqlite3* db, const string& name, const string& year, const string& city) {
    string sql = "INSERT INTO cliente (name, year, city) VALUES ('" + name + "', '" + year + "', '" + city + "');";

    char* mensagemErro;
    int exit = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &mensagemErro);

    if (exit != SQLITE_OK) {
        cerr << "Erro ao inserir cliente: " << mensagemErro << endl;
        sqlite3_free(mensagemErro);
    } else {
        cout << "Cliente inserido com sucesso!" << endl;
    }
}

void listarDB(sqlite3* db) {
    const char* sql = "SELECT * FROM cliente;";
    char* mensagemErro;

    int exit = sqlite3_exec(db, sql, callback, nullptr, &mensagemErro);

    if (exit != SQLITE_OK) {
        cerr << "Erro ao listar clientes: " << mensagemErro << endl;
        sqlite3_free(mensagemErro);
    }
}

void atualizarDB(sqlite3* db, int id, const string& name, const string& year, const string& city) {
    string sql = "UPDATE cliente SET name = '" + name + "', year = '" + year + "', city = '" + city + 
                 "' WHERE id = " + std::to_string(id) + ";";

    char* mensagemErro;
    int exit = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &mensagemErro);

    if (exit != SQLITE_OK) {
        cerr << "Erro ao atualizar o cliente: " << mensagemErro << endl;
        sqlite3_free(mensagemErro);
    } else {
        cout << "Cliente atualizado com sucesso!" << endl;
    }
}

void deletarProduto(sqlite3* db, int id) {
    std::string sql = "DELETE FROM cliente WHERE id = " + std::to_string(id) + ";";
    char* mensagemErro;

    int exit = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &mensagemErro);

    if (exit != SQLITE_OK) {
        std::cerr << "Erro ao deletar produto: " << mensagemErro << std::endl;
        sqlite3_free(mensagemErro);
    } else {
        std::cout << "Produto deletado com sucesso!" << std::endl;
    }
}


int main() {
    sqlite3* db;
    if (connectDB(db) != 0) {
        return -1;
    }

    int escolha;
    cout << "Escolha uma opção: \n1. Inserir produto\n2. Atualizar produto\n3. Listar produtos\n4. Deletar produto\n";
    cin >> escolha;

    if (escolha == 1) {
        string name, year, city;
        cout << "Nome: ";
        cin.ignore();
        getline(cin, name);
        cout << "Ano: ";
        getline(cin, year);
        cout << "Cidade: ";
        getline(cin, city);

        inserirDB(db, name, year, city);
    } else if (escolha == 2) {
        int id;
        string name, year, city;

        cout << "ID do produto a atualizar: ";
        cin >> id;
        cin.ignore();
        cout << "Novo Nome: ";
        getline(cin, name);
        cout << "Novo Ano: ";
        getline(cin, year);
        cout << "Nova Cidade: ";
        getline(cin, city);

        atualizarDB(db, id, name, year, city);
    } else if (escolha == 3) {
        listarDB(db);
    } else if (escolha == 4) {
        int id;
        cout << "ID do produto a deletar: ";
        cin >> id;

        deletarProduto(db, id);
    } else {
        cout << "Opção inválida!" << endl;
    }

    sqlite3_close(db);
    return 0;
}

