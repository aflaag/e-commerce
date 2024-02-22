#!/bin/sh

#sudo -u postgres psql postgres -f parameters.sql -f setup.sql -f schema.sql -f grant.sql -f triggers.sql
sudo -u postgres psql postgres -f parameters.sql -f setup.sql -f schema.sql -f grant.sql -f triggers.sql -f base_data.sql
#sudo -u postgres psql postgres -f parameters.sql -f triggers.sql