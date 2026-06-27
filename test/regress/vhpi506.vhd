entity vhpi506 is
end entity;

architecture test of vhpi506 is
    -- No signals: the VHPI plugin looks up the predefined TIME type by
    -- name and creates a signal of that physical type.
begin

    p1: process is
    begin
        wait for 1 ms;
        report "VHPI plugin did not end simulation" severity failure;
    end process;

end architecture;
