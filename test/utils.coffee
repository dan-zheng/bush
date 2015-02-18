Promise = require('bluebird')
util    = require('util')
exec    = require('child_process').exec

module.exports = self = { }

self.run = (cli, input) ->
  return new Promise (resolve, reject) ->
    exec "echo \"#{input}\" | #{cli}", (error, stdout, stderr) ->
      if (error) then reject(error);
      else resolve(stdout: stdout, stderr: stderr);

self.stripAnsi = (str) ->
  return str.replace(/\033\[[0-9;]+m/g, '')
