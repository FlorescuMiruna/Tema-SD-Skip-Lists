#include<bits/stdc++.h>

using namespace std;
ifstream f ("abce.in");
ofstream g ("abce.out");
class Nod
{
public:
    int inf;
    Nod **next; ///Il folosim pentru a retine pointer la nodul urmator pe mai multe niveluri
    Nod(int,int);
};
Nod::Nod(int inf,int nivel)
{
    this-> inf = inf;
    next = new Nod*[nivel+1];
    memset(next, 0, sizeof(Nod*)*(nivel+1)); ///Initializam vectorul de next cu 0

}
class SkipList
{

    int Nivel_Maxim; ///Nivelul maxim al listei
    float p; ///Fractia pentru nivel
    int nivel; ///Nivelul actual al listei
    Nod * prim;
public:
    SkipList(int,float);
    int nivelRandom();
    Nod *creaazaNod(int,int);
    void insereazaElement(int);
    void stergeElement (int);
    bool cautareElement(int);
    void afisareLista();
    void afisareListaPeNiveluri();
    void afisareNumere(int,int );
    Nod *predecesor(int);
    Nod *succesor(int);
};
SkipList::SkipList(int Nivel_Maxim,float p)
{
    this->Nivel_Maxim = Nivel_Maxim;
    this->p = p;
    nivel = 0;
    prim = new Nod(-1,Nivel_Maxim); ///Creez primul nod si il initializam cu -1

}
///Creez un nivel random pentru nod
int SkipList:: nivelRandom()
{
    float r = (float)rand()/RAND_MAX;

    int nivel = 0;
    while(nivel <= Nivel_Maxim && r<p) ///p va fi 0.5, deci vor fi sanse de 50% ca nivelul
                                       /// nodului sa creasca, "dam cu banul"
    {
        nivel++;
        r = (float)rand()/RAND_MAX;

    }

    return nivel;
}
Nod *SkipList::creaazaNod(int info,int nivel)
{
    Nod *n = new Nod(info,nivel);
    return n;
}
///Inserarea valorii in lista
void SkipList::insereazaElement(int inf)
{
    Nod *p = prim;
    ///Creez un array si il initializez cu 0
    Nod *v[Nivel_Maxim+1];
    memset(v,0,sizeof(Nod*)*Nivel_Maxim+1);
    ///Incepem de la cel mai inalt nivel si parcurgem lista, pentru a vedea unde inseram elementul

    for(int i = nivel; i>=0; i--)
    {
        while(p->next[i] != NULL && p->next[i]->inf<inf)   ///Ne uitam de fiecare data la next-ul elementului pe care suntem
            p = p->next[i];                                ///daca acesta este mai mare decat elementul de inserat inseamna ca nu
        v[i] = p;                                   ///putem merge mai departe pe nivelul respectiv si coboram un nivel
    }
    p = p->next[0];
    ///Inseram nodul intre v[0] si nodul curent,p

    ///Generam un nivel random pentru nod

    if (p == NULL || p->inf != inf)
    {

        int nivel_nou = nivelRandom();


        if(nivel_nou>nivel)
        {
            for(int i=nivel+1; i<=nivel_nou; i++)
                v[i] = prim;
            ///In cazul in care se genereaza un nivel mai mare decat nivelul actual al listei,
            ///Actualizam vectorul pentru urmatoarea data cand il vom folosi

            nivel =nivel_nou; ///Actualizam nivelul curent al listei
        }
        Nod *n = creaazaNod(inf,nivel_nou);

        for(int i=0; i<=nivel_nou; i++) ///Inseram nodul pe fiecare nivel pana la nivelul random ales
        {
            n->next[i] = v[i]->next[i];
            v[i]->next[i] = n;
        }

    }
}
void SkipList::afisareLista()
{
    cout<<"Lista este: \n";
    Nod *p =prim->next[0];
    while(p!=NULL)
    {
        cout<<p->inf<<" ";
        p = p->next[0];

    }
    cout<<"\n";


}

void SkipList::afisareListaPeNiveluri()
{
    cout<<"Lista este: \n";
    for(int i=0; i<=nivel; i++)
    {
        Nod *p =prim->next[i];
        cout<<"Nivelul "<<i<<": ";
        while(p!=NULL)
        {
            cout<<p->inf<<" ";
            p = p->next[i];

        }
        cout<<"\n";
    }

}
void SkipList::stergeElement(int inf)
{
    Nod *p = prim;
    Nod *v[Nivel_Maxim+1];
    memset(v,0,sizeof(Nod*)*(Nivel_Maxim+1));
    ///Parcurgem lista de la cel mai inalt nivel in jos pana ajungem
    ///la nodul de dinaintea celui inserat
    for(int i=nivel; i>=0; i--)
    {
        while (p->next[i] != NULL and p->next[i]->inf < inf)
            p = p->next[i];
        v[i] = p; ///Retinem pentru fiecare nivel ultimul nod de dinainte sa coboram pe nivelul urmator
    }
    p = p->next[0]; ///p va deveni nodul pe care vrem sa il stergem

    if(p!= NULL and p->inf == inf)
    {
        for(int i=0; i<=nivel; i++)
        {


            if(v[i]->next[i] != p)
                break; ///Daca pe acel nivel nodul next nu este nodul care trebuie sters, inseamna ca ne putem opri

            v[i]->next[i] = p->next[i]; ///Stergem nodul din lista pe fiecare nivel

        }
    }

    while(nivel>0 && prim->next[nivel] == 0)
        nivel--;
    ///Daca dupa stergerea nodului raman niveluri pe care nu sunt elemente le stergem

}
bool SkipList::cautareElement(int inf)
{
    bool ok=0;
    Nod *p = prim;
    for(int i=nivel; i>=0; i--)
    {
        while (p->next[i] && p->next[i]->inf <inf)
            p = p->next[i];
    }
    p = p ->next[0]; ///Acesta ar trebui sa fie nodul pe care il cautam, in caz ca se afla in lista
    if(p!= NULL and p->inf == inf)
    {
        ok = 1;
    }
    return ok;
}
void SkipList::afisareNumere(int x,int y)
{
    Nod *p = prim;
    for(int i=nivel; i>=0; i--)
    {
        while (p->next[i] && p->next[i]->inf <x)
            p = p->next[i];
    }
    p = p->next[0];
    g<<p->inf<<" ";
    while(p->next[0] && p->next[0]->inf <= y)
    {

        p=p->next[0];
        g<<p->inf<<" ";
    }
    g<<endl;


}
Nod * SkipList::predecesor(int x)
{
    Nod *p = prim;
    for(int i=nivel;i>=0;i--)
    {
        while (p->next[i] && p->next[i]->inf<=x)
            p=p->next[i];

    }
    return p;
}
Nod * SkipList::succesor(int x)
{
    Nod *p = prim;
    for(int i=nivel;i>=0;i--)
    {
        while (p->next[i] && p->next[i]->inf<x)
            p=p->next[i];

    }
    return p->next[0];
}


int n,x,y,operatie,k;
int main()
{
    srand((unsigned)time(0));
    f>>n;
    int m=n;
    while(m>1)   ///Vreau ca nivelul maxim sa fie log n
    {
        m=m/2;
        k++;
    }

    SkipList lista(k,0.5);


    for(int i=0; i<n; i++)
    {
    f>>operatie;
        if (operatie == 1)
        {
            f>>x;
            lista.insereazaElement(x);
        }

        if (operatie == 2)
        {
            f>>x;
            lista.stergeElement(x);

        }

        if (operatie == 3)
        {
            f>>x;
            g<<lista.cautareElement(x)<<"\n";
        }
        if(operatie == 4)
        {
            f>>x;
            g<<lista.predecesor(x)->inf<<"\n";
        }
        if(operatie == 5)
        {
            f>>x;
            g<<lista.succesor(x)->inf<<"\n";
        }
        if(operatie == 6)
        {
            f>>x>>y;
            lista.afisareNumere(x,y);
        }

    }

  //  lista.afisareListaPeNiveluri();
    f.close();
    g.close();
    return 0;

}
