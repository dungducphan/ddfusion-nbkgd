#!/bin/bash
# ./ddfusion ./excitationEnergyScan.mac

hadd ex_1.5MeV.root neutron_run00000_t*.root
hadd ex_2.0MeV.root neutron_run00001_t*.root
hadd ex_2.5MeV.root neutron_run00002_t*.root
hadd ex_3.0MeV.root neutron_run00003_t*.root
hadd ex_3.5MeV.root neutron_run00004_t*.root
hadd ex_4.0MeV.root neutron_run00005_t*.root

mv neutron_run0000*_t*.root archive

root Ana.C
