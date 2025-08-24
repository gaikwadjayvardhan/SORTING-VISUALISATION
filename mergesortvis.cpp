#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
using namespace std;

const int WIN_W = 2000;
const int WIN_H = 1200;

const int N = 100;   
int arr[N];
int tmp[N];


void draw(sf::RenderWindow &win, int* a, int n, int i1=-1, int i2=-1, int L=-1, int R=-1) {
    win.clear(sf::Color(20,20,20));

    int maxVal = 1;
    for(int i=0;i<n;i++) if(a[i]>maxVal) maxVal=a[i];

    float barW = (float)WIN_W / n;

    for(int i=0;i<n;i++) {
        float h = (a[i] / (float)maxVal) * (WIN_H-40);
        float x = i * barW;
        float y = WIN_H - h;

        sf::RectangleShape bar;
        bar.setPosition({x, y});  // ✅ SFML3 requires Vector2f
        bar.setSize({barW-1, h});

        sf::Color col(200,200,200);
        if(L!=-1 && R!=-1 && i>=L && i<=R) col = sf::Color(150,180,220);
        if(i==i1 || i==i2) col = sf::Color(250,100,80);

        bar.setFillColor(col);
        win.draw(bar);
    }

    win.display();
}

// keep window alive
bool checkEvents(sf::RenderWindow& win) {
    sf::Event e;
    while (win.pollEvent(e)) {
        if (e.type == sf::Event::Closed) {  
            win.close();
            return false;
        }
    }
    return true;
}



void merge(sf::RenderWindow &win, int* a, int L, int M, int R, int n) {
    int i=L, j=M+1, k=L;
    while(i<=M && j<=R) {
        if(!checkEvents(win)) return;
        if(a[i] <= a[j]) tmp[k++]=a[i++];
        else tmp[k++]=a[j++];
        draw(win,a,n,i,j,L,R);
        sf::sleep(sf::milliseconds(10));
    }
    while(i<=M) { tmp[k++]=a[i++]; draw(win,a,n,i,j,L,R); sf::sleep(sf::milliseconds(10)); }
    while(j<=R) { tmp[k++]=a[j++]; draw(win,a,n,i,j,L,R); sf::sleep(sf::milliseconds(10)); }

    for(int t=L;t<=R;t++) {
        if(!checkEvents(win)) return;
        a[t]=tmp[t];
        draw(win,a,n,t,-1,L,R);
        sf::sleep(sf::milliseconds(10));
    }
}


void msort(sf::RenderWindow &win, int* a, int L, int R, int n) {
    if(L>=R) return;
    int M=(L+R)/2;
    msort(win,a,L,M,n);
    msort(win,a,M+1,R,n);
    merge(win,a,L,M,R,n);
}

int main() {
    srand(time(0));
    for(int i=0;i<N;i++) arr[i]=20+(rand()%500);

    sf::RenderWindow win(sf::VideoMode({WIN_W, WIN_H}), "Merge Sort"); // ✅ Vector2u required
    win.setFramerateLimit(120);

    draw(win,arr,N);
    sf::sleep(sf::milliseconds(500));

    msort(win,arr,0,N-1,N);

    while(win.isOpen()) {
        if(!checkEvents(win)) break;
        draw(win,arr,N);
        sf::sleep(sf::milliseconds(16));
    }
    return 0;
}
