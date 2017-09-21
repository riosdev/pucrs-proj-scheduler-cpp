#include <iostream>
#include <list>
#include <vector>
#include <cstdint>

using namespace std;

enum class _Politica{FIFO,RR};

struct Tarefa{
    //variaveis
    char ID;
    uint16_t Prioridade, Chegada, Computacao;
    uint16_t Computado;
    _Politica Politica;
    //construtores
    Tarefa(char i, uint16_t c, uint16_t a, uint16_t p, _Politica o) : 
        ID(i),Computacao(c), Chegada(a), Prioridade(p), Politica(o), Computado(0)
    {}
    //metodos
    auto executar() -> void {Computado++;}
    auto c_restante() -> uint16_t {return Computacao - Computado;}
};

class Simulacao{
    //variaveis
    uint16_t Tempo;
    vector<Tarefa *> historico;
    vector<Tarefa *> tarefas;
    list<Tarefa *> *lista_prioridades[32];
  public:
    //construtores
    Simulacao() : Tempo(0){
        for (auto& q : lista_prioridades)
            q = new list<Tarefa *>();
    }
    //metodos
    auto AdicionarTarefa(Tarefa*) -> void;
    auto Passo() -> void;
    auto Ativa() -> bool;
    auto Imprimir() -> string;
    //destrutores
    ~Simulacao(){
        for(auto& q : lista_prioridades)
            delete q;
        for(auto& t : tarefas)
            delete t;
    }
};

auto main(void) -> int {
    vector<Simulacao *> simulacoes;
    int n;
    while(true){
        cin >> n;
        if(!n)break;
        auto s = n;
        auto simulacao = new Simulacao();
        do
        {
            uint16_t p, a, c, o;
            cin >> c >> a >> p >> o;
            auto tarefa = new Tarefa((s - n) + 'A', c, a, p, 
                (o == 1) ? _Politica::FIFO : _Politica::RR);
            simulacao->AdicionarTarefa(tarefa);
        } while (--n > 0);
        simulacoes.push_back(simulacao);
    }
    for(auto& s : simulacoes){
        while(s->Ativa())
            s->Passo();
        cout << s->Imprimir() << endl;
        delete s;
    }
}

auto Simulacao::AdicionarTarefa(Tarefa* t) -> void{
    if(t->Prioridade >= 1 && t->Prioridade <= 32){
        tarefas.push_back(t);
    }
}

auto Simulacao::Passo() -> void{
    for(auto& t : tarefas){
        if(t->Chegada == Tempo)
            lista_prioridades[t->Prioridade - 1]->push_front(t);
    }
    Tempo++;
    for (auto &q : lista_prioridades){
        if(q->size()){
            auto tarefa = q->front();
            tarefa->executar();
            historico.push_back(tarefa);
            if(tarefa->c_restante()){
                if(tarefa->Politica == _Politica::RR){
                    q->pop_front();
                    q->push_back(tarefa);
                }
            }else{
                q->pop_front();
            }
            return;
        }
    }
    historico.push_back(nullptr);
}

auto Simulacao::Ativa() -> bool{
    if(Tempo > 2048)
        return false;
    for (auto &t : tarefas)
        if(t->Chegada >= Tempo)
            return true;
    for (auto &q : lista_prioridades)
        if(q->size())
            return true;
    return false;
}

auto Simulacao::Imprimir() -> string{
    string str;
    str.reserve(Tempo);
    for(auto& t : historico)
        str += ((t == nullptr) ? '.' : t->ID);
    return str;
}