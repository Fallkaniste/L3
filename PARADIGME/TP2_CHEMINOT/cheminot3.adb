with TEXT_IO; use TEXT_IO;

procedure Cheminot2 is
package int_io is new Integer_io(integer);
use int_io;

pierre1:boolean:=FALSE;
pierre2:boolean:=FALSE;

task perou;
task body perou is

I:integer:=0;


begin

  --pour tester je prend 2 iterations par exemple et non pas une boucle infinie

  for I in 1..10 loop
    -- protocole d entree
    Put_Line("Peruvien arrete train et met sa pierre");
    pierre1:=TRUE;
    while(pierre2=TRUE) loop
      pierre1:=FALSE;
      Put_Line("Peruvien enleve sa pierre et fait la sieste");
    end loop;

  -- process1 entre en SC
  Put_Line("Peruvien passe");

  -- protocole de sortie
  Put_Line("Peruvien retourne au panier");
  Put_Line("Peruvien enleve sa pierre");
  pierre1:=FALSE;
  Put_Line("Peruvien repart");

  end loop;
end perou;


task bolivie;
task body bolivie is

J:integer:=0;

begin

  --pour tester je prend 2 iterations par exemple et non pas une boucle infinie

  for I in 1..10 loop
    -- protocole d entree
    Put_Line("Bolivien arrete train et met sa pierre");
    pierre2:=TRUE;
    while(pierre1=TRUE) loop
      pierre2:=FALSE;
      Put_Line("Bolivien enleve sa pierre et fait la sieste");
    end loop;

  -- process1 entre en SC
  Put_Line("Bolivien passe");

  -- protocole de sortie
  Put_Line("Bolivien retourne au panier");
  Put_Line("Bolivien enleve sa pierre");
  pierre2:=FALSE;
  Put_Line("Bolivien repart");

  end loop;
end bolivie;


begin
  Null;
end Cheminot2;
