library ieee;
use ieee.std_logic_1164.all;

entity vhpi502 is
end entity;

architecture test of vhpi502 is
    type rec_t is record
        a : std_logic;
        b : std_logic_vector(1 downto 0);
    end record;
    signal r : rec_t;
begin

    -- The VHPI plugin creates a new record signal, drives one of its
    -- fields, and ends the simulation.  This process is a watchdog.
    p1: process is
    begin
        wait for 1 ms;
        report "VHPI plugin did not end simulation" severity failure;
    end process;

end architecture;
