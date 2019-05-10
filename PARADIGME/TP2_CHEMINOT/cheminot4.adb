with TEXT_IO; use TEXT_IO;

procedure Cheminot3 is
package int_io is new Integer_io(integer);
use int_io;

pierre1:boolean:=FALSE;
pierre2:boolean:=FALSE;
tour:integer:=1;

task perou;
task body perou is

I:integer:=0;

begin

  for J in 1..10 loop
    -- protocole d entree
    Put_Line("Peruvien arrete son train et met sa pierre");
    pierre1:=TRUE;
    Put_Line("Péruvien donne le tour au Bolivien");
    tour:=2;

    --attente active
    while(pierre2=TRUE and tour=2) loop
      Put_Line("Peruvien fait la sieste");
    end loop;

    -- <SC>
    Put_Line("Peruvien passe");
    -- <SC>

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

  for I in 1..10 loop
    -- protocole d entree
    Put_Line("Bolivien arrete son train et met sa pierre");
    pierre2:=TRUE;
    Put_Line("Bolivien donne le tour au Peruvien");
    tour:=1;

    --attente active
    while(pierre1=TRUE and tour=1) loop
      Put_Line("Bolivien fait la sieste");
    end loop;

    -- <SC>
    Put_Line("Bolivien passe");
    -- <SC>

    -- protocole de sortie
    Put_Line("Bolivien retourne au panier");
    Put_Line("Bolivien enleve sa pierre");
    pierre2:=FALSE;
    Put_Line("Bolivien repart");
  end loop;
end bolivie;

begin
  Null;
end Cheminot3;
