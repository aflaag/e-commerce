#!/bin/sh

sudo -u postgres psql postgres -f parameters.sql -f setup.sql  -f schema.sql -f grant.sql