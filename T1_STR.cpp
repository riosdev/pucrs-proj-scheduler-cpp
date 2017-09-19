#include <iostream>
#include <queue>
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
    Tarefa(char i, uint16_t p, uint16_t a, uint16_t c, _Politica o) : 
        ID(i),Prioridade(p), Chegada(a), Computacao(c), Politica(o), Computado(0)
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
    queue<Tarefa *> *lista_prioridades[32];
  public:
    //construtores
    Simulacao() : Tempo(0){
        for (auto& q : lista_prioridades)
            q = new queue<Tarefa *>();
    }
    //metodos
    auto AdicionarTarefa(Tarefa&) -> void;
    auto Passo() -> void;
    auto Ativa() -> bool;
    auto Imprimir() -> string;
    //destrutores
    ~Simulacao(){
        for(auto& q : lista_prioridades)
            delete q;
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
            cin >> p >> a >> c >> o;
            auto tarefa = Tarefa((s - n) + 'A', p, a, c, 
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

auto Simulacao::AdicionarTarefa(Tarefa& t) -> void{
    if(t.Prioridade >= 1 && t.Prioridade <= 32){
        tarefas.push_back(&t);
        lista_prioridades[t.Prioridade - 1]->push(&t);
    }
}

auto Simulacao::Passo() -> void{
    Tempo++;
    for (auto& q : lista_prioridades)
    {
        if(q->size()){
            auto tarefa = q->front();
            tarefa->executar();
            historico.push_back(tarefa);
            if(tarefa->c_restante()){
                if(tarefa->Politica == _Politica::RR){
                    q->pop();
                    q->push(tarefa);
                }
            }else{
                q->pop();
            }
            return;
        }
    }
    historico.push_back(nullptr);
}

auto Simulacao::Ativa() -> bool{
    for(auto& q : lista_prioridades)
        if(q->size())
            return true;
    return false;
}

auto Simulacao::Imprimir() -> string{
    string str;
    str.reserve(Tempo);
    for(auto t : historico)
        str += ((t == nullptr) ? '.' : t->ID);
    return str;
}