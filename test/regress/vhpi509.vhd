library ieee;
use ieee.std_logic_1164.all;
use ieee.fixed_pkg.all;

entity vhpi509 is
end entity;

architecture test of vhpi509 is
    -- No signals: the VHPI plugin creates ufixed, sfixed, string, and
    -- bit_vector signals using the convenience wrappers.
begin

    p1: process is
    begin
        wait for 1 ms;
        report "VHPI plugin did not end simulation" severity failure;
    end process;

end architecture;
