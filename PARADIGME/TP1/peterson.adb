with TEXT_IO; use TEXT_IO;

--sauvegarder ce fichier sous le nom de: Dekker1.adb, et pour compiler lancer: gnatmake Dekker1.adb -o Dekker1

procedure Dekker5 is
package int_io is new Integer_io(integer);
use int_io;

tour:integer:=1;
demande1:boolean:=FALSE;
demande2:boolean:=FALSE;

task processus1;
task body processus1 is

I:integer:=0;


begin

  --pour tester je prend 2 iterations par exemple et non pas une boucle infinie

  for I in 1..2 loop
    -- protocole d entree
    demande1:=TRUE;
    tour:=2;
    while (demande2=TRUE AND tour/=1) loop
      Put_Line("Process1 en attente");
    end loop;

  -- process1 entre en SC
  Put_Line ("processus1 en SC");

  -- protocole de sortie
  demande1:=FALSE;

  end loop;
end processus1;


task processus2;
task body processus2 is

J:integer:=0;

Begin
--pour tester je prend 2 iterations et non pas une boucle infinie

for I in 1..2 loop
  -- protocole d entree
  demande2:=TRUE;
  tour:=1;
  while (demande1=TRUE AND tour/=2) loop
    Put_Line("Process2 en attente");
  end loop;

-- process1 entre en SC
Put_Line ("processus2 en SC");

-- protocole de sortie
demande2:=FALSE;

end loop;
end processus2;


begin
  Null;
end Dekker5;
