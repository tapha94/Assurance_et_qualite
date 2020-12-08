#include <cstdlib> // pour std::rand()
#include <iostream>

#include <fonctions.hpp>
#include <longueur.cpp>

// implementations alternatives pour verifier votre code

////////////////////////////////////////
#include <cstring> // std::strlen

////////////////////////////////////////
int cpp_max_int(const int t[], size_t nb)
 {
  int max_so_far=t[0]; // regle le probleme de la valeur initiale
  for (size_t i=1;i<nb;i++)
   if (t[i]>max_so_far)
    max_so_far=t[i];

  return max_so_far;
 }

////////////////////////////////////////
void cpp_t_to_hms(uint64_t t, uint64_t & h, uint64_t & m, uint64_t & s)
 {
  s=t % 60; // %, c'est modulo
  t/=60;
  m=t % 60;
  h=t/60;
 }

////////////////////////////////////////
void cpp_hms_to_t(uint64_t h, uint64_t m, uint64_t s, uint64_t & t)
 {
  t=h*3600+m*60+s;
 }


int main()
 {
  // test asm_strlen
  std::string mots[]{
    "schtroumpfique",
    "albatros",
    "",
    "emberlificote",
    "pleonasme",
    "amphigouri",
    "sesquipedien",
    "ailurophile",
    "supercalifragilisticexpidelilicieux",
    "dextrogyre"
     };

  for ( const std::string & m : mots)
   {
    size_t l1=std::strlen(m.c_str());
    size_t l2=asm_strlen(m.c_str());
    if (l1!=l2)
     {
      std::cout << m << ' ' << l1 << " donne " << l2 << std::endl;
      return 1; // error!
     }
   }

  // test max_int
  for (int i=0;i<10;i++)
   {
    const int nb_ints=20;
    int t[nb_ints];
    for (int j=0;j<nb_ints;j++) t[j]=std::rand() % 100; // parce que.
    int max_1=cpp_max_int(t,nb_ints);
    int max_2=asm_max_int(t,nb_ints);
    if (max_1!=max_2)
     {
      std::cout << "[";
      for (auto i:t) std::cout <<  ' ' << i;
      std::cout << " ] : " << max_1 << " donne " << max_2 << std::endl;
      return 1; // error!
     }
   }
  
  
  // test hms/t
  for (uint64_t h=0;h<24;h++)
   for (uint64_t m=0;m<60;m++)
    for (uint64_t s=0;s<60;s++)
     {
      // test "implementation de reference"
      uint64_t t,h2,m2,s2;
      cpp_hms_to_t(h,m,s,t);
      cpp_t_to_hms(t,h2,m2,s2);
      if ((h2!=h) || (m2!=m) || (s2!=s))
       {
        std::cout
         << "HMS(ref): " << h << ':' << m << ':' << s
         << " donne " << h2 << ':' << m2 << ':' << s2
         << std::endl;
        return 1; // error!
       }

      // test croise 1: encode c++, decode asm
      cpp_hms_to_t(h,m,s,t);
      asm_t_to_hms(t,h2,m2,s2);
      if ((h2!=h) || (m2!=m) || (s2!=s))
       {
        std::cout
         << "Vrai est "  << h << ':' << m << ':' << s
         << " Asm donne " << h2 << ':' << m2 << ':' << s2
         << std::endl;
        return 1; // error!
       }

      // test croise 2: encode asm, decode c++
      asm_hms_to_t(h,m,s,t);
      cpp_t_to_hms(t,h2,m2,s2);
      if ((h2!=h) || (m2!=m) || (s2!=s))
       {
        std::cout
         << "Vrai est "  << h << ':' << m << ':' << s
         << " Asm donne " << h2 << ':' << m2 << ':' << s2
         << std::endl;
        return 1; // error!
       }

      // dernier test: tout asm
      asm_hms_to_t(h,m,s,t);
      asm_t_to_hms(t,h2,m2,s2);
      if ((h2!=h) || (m2!=m) || (s2!=s))
       {
        std::cout
         << "HMS(ref): " << h << ':' << m << ':' << s
         << " donne " << h2 << ':' << m2 << ':' << s2
         << std::endl;
        return 1; // error!
       }
      
     }
  return 0; // success!
 }
