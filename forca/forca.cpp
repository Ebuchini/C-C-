#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <cctype>

using namespace std;

// Carrega palavras do arquivo para um vetor
vector<string> carregarPalavras(const string& nomeArquivo) {
    vector<string> palavras;
    ifstream arquivo(nomeArquivo);
    string palavra;

    if (!arquivo.is_open()) {
        cerr << "Erro: nao foi possivel abrir o arquivo '" << nomeArquivo << "'\n";
        exit(1);
    }

    while (getline(arquivo, palavra)) {
        if (!palavra.empty()) {
            // Remover quebras de linha e espaços extras
            palavra.erase(remove_if(palavra.begin(), palavra.end(), ::isspace), palavra.end());
            palavras.push_back(palavra);
        }
    }

    arquivo.close();
    return palavras;
}

// Sorteia uma palavra do vetor
string escolherPalavraAleatoria(const vector<string>& palavras) {
    srand(static_cast<unsigned int>(time(nullptr)));
    int indice = rand() % palavras.size();
    return palavras[indice];
}

// Exibe o estado atual do jogo
void mostrarEstado(const string& exibida, const vector<char>& usadas, int tentativas) {
    cout << "\nPalavra: ";
    for (char c : exibida) cout << c << ' ';

    cout << "\nLetras usadas: ";
    for (char l : usadas) cout << l << ' ';

    cout << "\nTentativas restantes: " << tentativas << "\n";
}

bool letraJaUsada(const vector<char>& usadas, char letra) {
    return find(usadas.begin(), usadas.end(), letra) != usadas.end();
}

bool jogoGanho(const string& exibida) {
    return exibida.find('_') == string::npos;
}

int main() {
    vector<string> palavras = carregarPalavras("palavras.txt");

    if (palavras.empty()) {
        cerr << "Erro: o arquivo esta vazio ou mal formatado.\n";
        return 1;
    }

    string palavra = escolherPalavraAleatoria(palavras);
    string exibida(palavra.length(), '_');
    vector<char> usadas;
    int tentativas = 6;
    char palpite;

    cout << "=== JOGO DA FORCA ===\n";

    while (tentativas > 0 && !jogoGanho(exibida)) {
        mostrarEstado(exibida, usadas, tentativas);
        cout << "\nDigite uma letra: ";
        cin >> palpite;
        palpite = tolower(palpite);

        if (!isalpha(palpite)) {
            cout << "Digite apenas letras.\n";
            continue;
        }

        if (letraJaUsada(usadas, palpite)) {
            cout << "Letra ja usada.\n";
            continue;
        }

        usadas.push_back(palpite);
        bool acertou = false;

        for (size_t i = 0; i < palavra.length(); ++i) {
            if (palavra[i] == palpite) {
                exibida[i] = palpite;
                acertou = true;
            }
        }

        if (acertou)
            cout << "Boa! Letra correta.\n";
        else {
            cout << "Errou!\n";
            tentativas--;
        }
    }

    if (jogoGanho(exibida))
        cout << "\nParabens voce venceu! A palavra era: " << palavra << endl;
    else
        cout << "\nVoce perdeu Tente novamente. A palavra era: " << palavra << endl;
    
    return 0;
}