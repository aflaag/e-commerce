#!/bin/sh

sudo -u postgres psql postgres -f parameters.sql -f setup.sql -f schemas.sql -f permissions.sql -f triggers.sql -f fill.sql -f tests.sql -f setup_log.sql -f schemas_log.sql -f permissions_log.sql -f triggers_log.sql
