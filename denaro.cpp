#include "denaro.h"
#include <vector>
#include <cmath>
#include <random>
#include <fstream>
#include <iostream>

void Person::SetMoney(int S){  //attenzione, SetMoney aumenta di S i soldi, non li setta a S
    if (money_ == 0 && S<0) { //non posso andare sottozero
    } else {
        money_ += S;
    }
}

void Person::Move( Way &direction, int l) {
    int L = l*l; //dim della griglia
    if (direction == Way::Right && x_%l != l-1 ) { x_++; }
    if (direction == Way::Left && x_&l != 0) { x_--; }
    if (direction == Way::Up && x_/l != 0) { x_ -= l; }
    if (direction == Way::Down && x_/l != l-1) { x_ += l; }
}

void SetGrid(std::vector<Person> &Grid, int l, int Cap){   
    int L = l*l; //dimensione della griglia, l è il lato
    std::random_device seed;                        
    std::uniform_int_distribution<int> positionDist(0, L);
    std::uniform_int_distribution<int> moneyDist(0, Cap);
    for(int i=0; i<Grid.size(); ++i){  //per ogni persona
        int X = positionDist(seed);
        int M = moneyDist(seed);
        Grid[i].SetPosition(X);         //setta le posizioni con numeri random generati da 0 a L
        Grid[i].SetMoney(M);          //setta il denaro con numeri random generati da 0 a Cap
    }
} 

void Gamble(double &p, Person& p1, Person& p2){
    if (p<0.5) { 
        //vince p1
        if(p2.GetMoney() != 0){
            p1.SetMoney(+1);   
            p2.SetMoney(-1);    //il caso in cui p2 perde ma è povero è già controllato in SetMoney (non perde nulla)  
        } else {}                //se p2 è povero p1 non vince nulla                    
    } else if (p>0.5) { 
        //vince p2
        if(p1.GetMoney() != 0){
            p2.SetMoney(+1); 
            p1.SetMoney(-1); 
        } else {}       
    }
}

void Evolve(int F, int l, int N, int Cap){   //F sono i frame, l è il lato della griglia, 
                                            //N è il numero di persone, Cap il capitale massimo
    std::vector<Person> Grid(N);
    SetGrid(Grid, l, Cap);

    std::random_device seed;
    std::uniform_int_distribution<int> dist1(1, 4);
    std::uniform_real_distribution<double> dist2(0.0,1.0);

    std::ofstream Fout("Model2.txt"); //file per salvare i dati

    for(int t=0; t<F; ++t){  //dinamica 

        for(int i=0; i<N; ++i){    //movimento degli individui nella griglia
            int pWay = dist1(seed);
            Way direction;
            if (pWay==1) {
                direction = Way::Right;
            } else if (pWay==2){
                direction = Way::Left;
            } else if (pWay==3){
                direction = Way::Up;
            } else if (pWay==4){
                direction = Way::Down;
            }
            Grid[i].Move(direction, l);
        }
        //prima si muovono tutte le particelle di uno step, poi giocano tra loro se hanno stessa posizione
        for (int i=0; i<N; ++i){
            for (int j=i+1; j<N; ++j){  
                if( Grid[i].GetPosition() == Grid[j].GetPosition() ) {
                    double pGamble = dist2(seed);
                    Gamble(pGamble, Grid[i], Grid[j]);
                }
            }
        }
        if(t%1000 == 0) { std::cout<<t<<" frames completed"<<'\n'; } 
    }

    std::cout<<"simulation run succesfully"<<'\n';
    //for che salva i dati della simulazione, a simulazione finita(**mi interessa l'equilibrio non la dinamica!**); incolonna Soldi(m) e persone con m soldi (NofM)
    for (int m=0; m<Cap; ++m){
        int NofM=0; 
        for (int i=0; i<N; ++i) {
            if (Grid[i].GetMoney()==m) { NofM += 1; }
        }
        Fout<<m<<'\t'<<NofM<<'\n';
    }
    Fout.close();
    std::cout<<"data saved in file Model2.txt"<<'\n';
}
