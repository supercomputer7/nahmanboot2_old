#include "MCFG.h"

MCFG::~MCFG()
{

}
void MCFG::initialize(ACPI_MCFG* rsdt)
{
    this->mcfg_table = (ACPI_MCFG*)rsdt;
}
ACPI_MCFG* MCFG::get_table()
{
    return this->mcfg_table;
}
