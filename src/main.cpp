#include <cstdlib>
#include <dpp/dpp.h>
#include <format>

// https://stackoverflow.com/a/478960
std::string exec(std::string cmd) {
  std::array<char, 512> buffer;
  std::string result;
  std::unique_ptr<FILE, void (*)(FILE *)> pipe(popen(cmd.c_str(), "r"),
                                               [](FILE *f) -> void {
                                                 // wrapper to ignore the return
                                                 // value from pclose() is
                                                 // needed with newer versions
                                                 // of gnu g++
                                                 std::ignore = pclose(f);
                                               });
  if (!pipe) {
    throw std::runtime_error("popen() failed!");
  }
  while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) !=
         nullptr) {
    result += buffer.data();
  }
  return result;
}

int main() {

  dpp::cluster bot(std::getenv("BOT_TOKEN"));

  bot.on_slashcommand([](auto event) {
    if (event.command.get_command_name() != "cowsay") {
      return;
    }
    std::string cmdout = exec("fortune|cowsay");
    event.reply(std::format("```\n{}\n```", cmdout));
  });

  bot.on_ready([&bot](auto event __attribute__((unused))) {
    if (dpp::run_once<struct register_bot_commands>()) {
      bot.global_command_create(
          dpp::slashcommand("cowsay", "tylko jedno w głowie mam", bot.me.id));
    }
  });

  bot.start(dpp::st_wait);
  return 0;
}
