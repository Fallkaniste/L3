with TEXT_IO; use TEXT_IO;

--sauvegarder ce fichier sous le nom de: Dekker1.adb, et pour compiler lancer: gnatmake Dekker1.adb -o Dekker1

procedure Dekker2 is
package int_io is new Integer_io(integer);
use int_io;

P1inside:boolean:=FALSE;
P2inside:boolean:=FALSE;

task processus1;
task body processus1 is

I:integer:=0;


begin

  --pour tester je prend 2 iterations par exemple et non pas une boucle infinie


  for I in 1..2 loop
    -- protocole d entree
    while (P2inside) loop
      Put_Line("Process1 en attente");
    end loop;

  P1inside:=TRUE;
  -- process1 entre en SC
  Put_Line ("processus1 en SC");

  -- protocole de sortie
  P1inside:=FALSE;

  end loop;
end processus1;


task processus2;
task body processus2 is

J:integer:=0;

Begin
--pour tester je prend 2 iterations et non pas une boucle infinie

  for J in 1..2 loop
  -- protocole d entree
  while (P1inside) loop
    Put_Line("Process1 en attente");
  end loop;

  P2inside:=TRUE;

  -- process2 entre en SC
  Put_Line ("processus2 en SC");

  -- protocole de sortie
  P2inside:=FALSE;

  end loop;
end processus2;


begin
  Null;
end Dekker2;
