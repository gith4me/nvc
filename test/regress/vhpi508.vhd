library ieee;
use ieee.std_logic_1164.all;

entity vhpi508 is
end entity;

architecture test of vhpi508 is
    -- No signals: the VHPI plugin uses the convenience wrappers to
    -- create std_logic and std_logic_vector signals.
begin

    p1: process is
    begin
        wait for 1 ms;
        report "VHPI plugin did not end simulation" severity failure;
    end process;

end architecture;
