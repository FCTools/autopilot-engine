# Autopilot Engine

<p align="center">
<a href="https://t.me/alcatraz_rm"><img src="https://img.shields.io/badge/Telegram Chat-@alcatraz_rm-2CA5E0.svg?logo=telegram&style=for-the-badge" alt="Chat on Telegram"/></a>
<img src="https://img.shields.io/badge/version-v.1.0.0_alpha.133-green?style=for-the-badge" alt="Last Release"/>
</p>

## Autopilot engine (C++ based conditions checking module)
Project name for copyright: autopilot.autopilot-engine

Project documentation (old, no longer maintained): [doc](https://docs.google.com/document/d/1Z79HDBINUZZ2xw2-kPKdatbatP-Nh94qgwU8WsKzlEI/edit)
<br>
YouTrack Documentation (current): [doc](https://fcteam.myjetbrains.com/youtrack/articles/FCTOOLSDEV-A-2/%D0%90%D0%B2%D1%82%D0%BE%D0%BF%D0%B8%D0%BB%D0%BE%D1%82---%D0%B8%D0%BD%D1%84%D0%BE%D1%80%D0%BC%D0%B0%D1%86%D0%B8%D1%8F) 

Supported functions (optimization options):
* Play/stop campaign
* Include/exclude zones

Supported traffic sources:
* Propeller Ads ([api documentation](https://ssp-api.propellerads.com/v5/docs/#/))
* Evadav ([api documentation](https://evadav.com/docs/api#/))

Stack:
* [PostgreSQL](https://www.postgresql.org/) as main database (version: >= 12.5)
* [pqxx](http://pqxx.org/development/libpqxx/) library for PostgreSQL (version: >= 6.4.5-2build1)
* [cpp_redis](https://github.com/cpp-redis/cpp_redis) library for Redis (version: >= 4.3.1) and tacopie (requirement for cppredis)
* [curlpp](http://www.curlpp.org/) library (version: >= 0.8.1) and [curl](https://curl.se/) (version: >= 7.68.0-1ubuntu2.4) for http-requests
* [spdlog](https://github.com/gabime/spdlog) library for thread-safe logging (version: >= 1.8.2)
* [CMake](https://cmake.org/) for building (version: >= 3.16.3)

<br>
<br>
<p align="center">
Copyright © 2020-2021
<br>
Filthy Claws Tools - All Rights Reserved
</p>
