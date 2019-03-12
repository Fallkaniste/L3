with TEXT_IO; use TEXT_IO;

procedure Cheminot1 is
package int_io is new Integer_io(integer);
use int_io;

pierre:boolean:=FALSE;

task perou;
task body perou is

I:integer:=0;


begin

  --pour tester je prend 2 iterations par exemple et non pas une boucle infinie

  for I in 1..10 loop
    -- protocole d entree
    Put_Line("Peruvien arrete train");
    while(pierre=TRUE) loop
      Put_Line("Peruvien fait la sieste");
    end loop;
    Put_Line("Peruvien cherche pierre");
    Put_Line("Peruvien met pierre dans panier");
    pierre:=TRUE;

  -- process1 entre en SC
  Put_Line("Peruvien passe");

  -- protocole de sortie
  Put_Line("Peruvien retourne au panier");
  Put_Line("Peruvien enleve la pierre");
  pierre:=FALSE;
  Put_Line("Peruvien repart");

  end loop;
end perou;


task bolivie;
task body bolivie is

J:integer:=0;

begin

  --pour tester je prend 2 iterations par exemple et non pas une boucle infinie

  for J in 1..10 loop
    -- protocole d entree
    Put_Line("Bolivien arrete train");
    while(pierre=TRUE) loop
      Put_Line("Bolivien fait la sieste");
    end loop;
    Put_Line("Bolivien cherche pierre");
    Put_Line("Bolivien met pierre dans panier");
    pierre:=TRUE;

  -- process1 entre en SC
  Put_Line("Bolivien passe");

  -- protocole de sortie
  Put_Line("Bolivien retourne au panier");
  Put_Line("Bolivien enleve la pierre");
  pierre:=FALSE;
  Put_Line("Bolivien repart");

  end loop;
end bolivie;


begin
  Null;
end Cheminot1;
