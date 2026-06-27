library ieee;
use ieee.std_logic_1164.all;

entity vhpi503 is
end entity;

architecture test of vhpi503 is
    signal x : std_logic := '0';
begin

    -- The VHPI plugin creates a new region containing a new signal,
    -- drives it, and ends the simulation.  This process is a watchdog.
    p1: process is
    begin
        wait for 1 ms;
        report "VHPI plugin did not end simulation" severity failure;
    end process;

end architecture;
