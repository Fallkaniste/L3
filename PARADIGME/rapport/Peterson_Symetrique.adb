with TEXT_IO; use TEXT_IO;


procedure Peterson_Symetrique is
package int_io is new Integer_io(integer);
use int_io;


--Type Tab is array(0..1) of Boolean; -- Je cré un type Tab qui est un tableau avec 2 booleen (vrai ou faux).
--Demand(FALSE, FALSE): Tab; --Je cré mon tableau tab.

Demand: array(0..1) of Boolean := (others => FALSE);--Je cré un tableau de booleen.
Tour: Integer;
J: Integer;

subtype Process_num is Integer range 0..1; -- Je créer un sous type d'entier ne comprenant que 1 et 0.
subtype NumTour is Integer range 1..10; -- Je créer un sous type d'entier ne comprenant l'inteval de 1 a 5.

procedure Entre(I: in Process_num) is -- En entrant dans la procedure Entree je met I a la valeur actuel de Process_num.
begin
  J:=(I+1) mod 2;
  Demand(i):=TRUE;
  Tour:=J;
  while (Demand(J)=True and Tour /= 1) loop null;
end loop;
end Entre;

procedure Sortie(I: in Process_num) is
begin
  Demand(1):=FALSE;
end Sortie;


task Peterson1;
task body Peterson1 is

  begin
    for I in NumTour loop
      Entre(0);
      Put_Line("Processus 1 en Section Critique");
      Sortie(0);
    end loop;
  end Peterson1;

  task Peterson2;
  task body Peterson2 is

    begin
      for I in NumTour loop
        Entre(1);
        Put_Line("Processus 2 en Section Critique");
        Sortie(1);
      end loop;
    end Peterson2;

  begin
    Null;
  end Peterson_Symetrique;
