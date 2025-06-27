import { Button, Card, CardContent, Chip } from "@mui/material"
import { Play, Users, Map, Zap, Download, Github, Monitor, Gamepad2, Target, Shield } from "lucide-react"

export default function GamePage() {
  const handleGithub = () => {
    window.open("https://github.com/Sebakrag/taller-CounterStrike", "_blank");
  };

  const handleDownload = () => {
    window.open(
      "https://github.com/Sebakrag/taller-CounterStrike/archive/refs/tags/v1.0.0.zip",
      "_blank"
    )
  }

  return (
    <div className="min-h-screen bg-slate-950 text-white">
      {/* Header */}
      <header className="border-b border-slate-800 bg-slate-950/80 backdrop-blur-sm sticky top-0 z-50">
        <div className="container mx-auto px-4 py-4 flex items-center justify-between">
          <div className="flex items-center space-x-2">
            <Target className="h-8 w-8 text-red-500" />
            <span className="text-2xl font-bold">CS2D Strike</span>
          </div>
          <nav className="hidden md:flex items-center space-x-6">
            <a href="#features" className="hover:text-red-400 transition-colors">
              Características
            </a>
            <a href="#gameplay" className="hover:text-red-400 transition-colors">
              Jugabilidad
            </a>
            <a href="#download" className="hover:text-red-400 transition-colors">
              Descargar
            </a>
          </nav>
          <Button className="bg-red-600 hover:bg-red-700"
            sx={{
              backgroundColor: '#dc2626',
              color: 'white',
              textTransform: 'none',
              gap: '5px',
              fontSize: '1.1rem',
              borderRadius: '12px',
              paddingLeft: '15px',
              paddingRight: '15px',
              '&:hover': {
                backgroundColor: '#b91c1c'
              }
            }}
            onClick={handleDownload}
          >
            <Download className="h-4 w-4 mr-2" />
            Jugar Ahora
          </Button>
        </div>
      </header>

      {/* Hero Section */}
      <section className="relative py-20 lg:py-32 overflow-hidden">
        <div className="absolute inset-0 bg-gradient-to-br from-red-900/20 via-slate-950 to-blue-900/20" />
        <div className="container mx-auto px-4 relative z-10">
          <div className="grid lg:grid-cols-2 gap-12 items-center">
            <div className="space-y-8">
              <div className="space-y-4">
                <Chip className="bg-red-600/20 text-red-400 border-red-600/30"
                  label="Shooter Táctico Multijugador"
                  sx={{ backgroundColor: '#370f1b', color: '#f27171', fontWeight: 'bold', border: 'solid #f27171', borderWidth: 'thin' }}
                />
                <h1 className="text-5xl lg:text-7xl font-bold leading-tight">
                  CS2D
                  <span className="text-red-500"> Strike</span>
                </h1>
                <p className="text-xl text-slate-300 leading-relaxed">
                  Experimenta combate táctico 2D intenso en este shooter multijugador inspirado en Counter-Strike. Crea
                  partidas, elige tus mapas y domina el campo de batalla con jugabilidad estratégica.
                </p>
              </div>
              <div className="flex flex-col sm:flex-row gap-4">
                <Button className="bg-red-600 hover:bg-red-700 text-lg px-8 py-6"
                  sx={{
                    backgroundColor: '#dc2626',
                    color: 'white',
                    textTransform: 'none',
                    fontSize: '1.1rem',
                    borderRadius: '12px',
                    padding: '10px 20px',
                    fontWeight: 'bold',
                    '&:hover': {
                      backgroundColor: '#b91c1c'
                    }
                  }}
                  onClick={handleDownload}
                >
                  <Play className="h-5 w-5 mr-2" />
                  Empezar a Jugar
                </Button>
                <Button
                  className="border-slate-600 text-slate-300 hover:bg-slate-800 text-lg px-8 py-6"
                  sx={{
                    backgroundColor: '#ffffff',
                    color: 'black',
                    textTransform: 'none',
                    fontSize: '1.1rem',
                    borderRadius: '12px',
                    padding: '10px 20px',
                    fontWeight: 'bold',
                    border: 'thin solid #FFF',
                    transition: '0.3s',
                    '&:hover': {
                      backgroundColor: '#1e293b',
                      color: "#FFF",
                      border: 'thin solid #FFF'
                    }
                  }}
                  onClick={handleGithub}
                >
                  <Github className="h-5 w-5 mr-2" />
                  Ver Código
                </Button>
              </div>
              <div className="flex items-center space-x-8 text-sm text-slate-400">
                <div className="flex items-center space-x-2">
                  <Users className="h-4 w-4" />
                  <span>Multijugador</span>
                </div>
                <div className="flex items-center space-x-2">
                  <Monitor className="h-4 w-4" />
                  <span>Multiplataforma</span>
                </div>
                <div className="flex items-center space-x-2">
                  <Zap className="h-4 w-4" />
                  <span>Ritmo Rápido</span>
                </div>
              </div>
            </div>
            <div className="relative">
              <div className="absolute inset-0 bg-gradient-to-r from-red-500/20 to-blue-500/20 rounded-2xl blur-3xl" />
              <video
                autoPlay
                loop
                muted
                playsInline
                className="relative z-10 rounded-2xl border border-slate-700 shadow-2xl"
                width={800}
                height={600}
              >
                <source src="src/assets/demo.mp4" type="video/mp4" />
                Tu navegador no soporta videos en HTML5.
              </video>
            </div>
          </div>
        </div>
      </section>

      {/* Features Section */}
      <section id="features" className="py-20 bg-slate-900/50">
        <div className="container mx-auto px-4">
          <div className="text-center mb-16">
            <h2 className="text-4xl font-bold mb-4">Características del Juego</h2>
            <p className="text-xl text-slate-400 max-w-2xl mx-auto">
              Construido con tecnología moderna y mecánicas clásicas de shooter táctico
            </p>
          </div>
          <div className="grid md:grid-cols-2 lg:grid-cols-3 gap-8">
            <Card className="bg-slate-800/50 border-slate-700 hover:bg-slate-800/70 transition-colors"
              sx={{ backgroundColor: '#131c2e', borderRadius: '15px', padding: '15px' }}
            >
              <CardContent className="p-6">
                <Users className="h-12 w-12 text-red-500 mb-4" />
                <h3 className="text-xl font-semibold mb-2" style={{ color: '#FFF' }}>Partidas Multijugador</h3>
                <p className="text-slate-400">
                  Hospeda o únete a partidas multijugador con amigos. Soporte para múltiples juegos y jugadores
                  simultáneos.
                </p>
              </CardContent>
            </Card>
            <Card className="bg-slate-800/50 border-slate-700 hover:bg-slate-800/70 transition-colors"
              sx={{ backgroundColor: '#131c2e', borderRadius: '15px', padding: '15px' }}
            >
              <CardContent className="p-6">
                <Map className="h-12 w-12 text-blue-500 mb-4" />
                <h3 className="text-xl font-semibold mb-2" style={{ color: '#FFF' }}>Múltiples Mapas</h3>
                <p className="text-slate-400">
                  Elige entre varios mapas tácticos, cada uno diseñado para jugabilidad estratégica y tiroteos intensos.
                </p>
              </CardContent>
            </Card>
            <Card className="bg-slate-800/50 border-slate-700 hover:bg-slate-800/70 transition-colors"
              sx={{ backgroundColor: '#131c2e', borderRadius: '15px', padding: '15px' }}
            >
              <CardContent className="p-6">
                <Gamepad2 className="h-12 w-12 text-green-500 mb-4" />
                <h3 className="text-xl font-semibold mb-2" style={{ color: '#FFF' }}>Controles Fáciles</h3>
                <p className="text-slate-400">
                  Controles 2D intuitivos que son fáciles de aprender pero difíciles de dominar. Perfecto para juego
                  competitivo.
                </p>
              </CardContent>
            </Card>
            <Card className="bg-slate-800/50 border-slate-700 hover:bg-slate-800/70 transition-colors"
              sx={{ backgroundColor: '#131c2e', borderRadius: '15px', padding: '15px' }}
            >
              <CardContent className="p-6">
                <Shield className="h-12 w-12 text-purple-500 mb-4" />
                <h3 className="text-xl font-semibold mb-2" style={{ color: '#FFF' }}>Jugabilidad Táctica</h3>
                <p className="text-slate-400">
                  Combate estratégico basado en equipos con mecánicas clásicas de Counter-Strike adaptadas para
                  jugabilidad 2D.
                </p>
              </CardContent>
            </Card>
            <Card className="bg-slate-800/50 border-slate-700 hover:bg-slate-800/70 transition-colors"
              sx={{ backgroundColor: '#131c2e', borderRadius: '15px', padding: '15px' }}
            >
              <CardContent className="p-6">
                <Zap className="h-12 w-12 text-yellow-500 mb-4" />
                <h3 className="text-xl font-semibold mb-2" style={{ color: '#FFF' }}>Acción en Tiempo Real</h3>
                <p className="text-slate-400">
                  Acción multijugador en tiempo real de ritmo rápido con redes fluidas y jugabilidad responsiva.
                </p>
              </CardContent>
            </Card>
            <Card className="bg-slate-800/50 border-slate-700 hover:bg-slate-800/70 transition-colors"
              sx={{ backgroundColor: '#131c2e', borderRadius: '15px', padding: '15px' }}
            >
              <CardContent className="p-6">
                <Monitor className="h-12 w-12 text-cyan-500 mb-4" />
                <h3 className="text-xl font-semibold mb-2" style={{ color: '#FFF' }}>Arquitectura Servidor-Cliente</h3>
                <p className="text-slate-400">
                  Arquitectura robusta servidor-cliente que soporta múltiples partidas y jugadores simultáneos.
                </p>
              </CardContent>
            </Card>
          </div>
        </div>
      </section>

      {/* Gameplay Screenshots */}
      <section id="gameplay" className="py-20">
        <div className="container mx-auto px-4">
          <div className="text-center mb-16">
            <h2 className="text-4xl font-bold mb-4">Capturas de Jugabilidad</h2>
            <p className="text-xl text-slate-400">Mira la acción en nuestro shooter táctico 2D</p>
          </div>
          <div className="grid md:grid-cols-2 lg:grid-cols-3 gap-6">
            <div className="relative group overflow-hidden rounded-xl">
              <img
                src="src/assets/menu.png"
                alt="Lobby de Partida"
                width={400}
                height={400}
                className="w-full h-64 object-cover transition-transform group-hover:scale-105"
              />
              <div className="absolute inset-0 bg-gradient-to-t from-black/60 to-transparent" />
              <div className="absolute bottom-4 left-4">
                <h3 className="text-lg font-semibold">Lobby de Partida</h3>
                <p className="text-sm text-slate-300">Crea y únete a partidas</p>
              </div>
            </div>
            <div className="relative group overflow-hidden rounded-xl">
              <img
                src="src/assets/match9.png"
                alt="Acción en el Juego"
                width={400}
                height={400}
                className="w-full h-64 object-cover transition-transform group-hover:scale-105"
              />
              <div className="absolute inset-0 bg-gradient-to-t from-black/60 to-transparent" />
              <div className="absolute bottom-4 left-4">
                <h3 className="text-lg font-semibold">Combate Táctico</h3>
                <p className="text-sm text-slate-300">Tiroteos 2D intensos</p>
              </div>
            </div>
            <div className="relative group overflow-hidden rounded-xl">
              <img
                src="src/assets/editor.png"
                alt="Selección de Mapa"
                width={400}
                height={400}
                className="w-full h-64 object-cover transition-transform group-hover:scale-105"
              />
              <div className="absolute inset-0 bg-gradient-to-t from-black/60 to-transparent" />
              <div className="absolute bottom-4 left-4">
                <h3 className="text-lg font-semibold">Editor de Mapas</h3>
                <p className="text-sm text-slate-300">Crea tu propio campo de batalla</p>
              </div>
            </div>
          </div>
        </div>
      </section>

      {/* How to Play */}
      <section className="py-20 bg-slate-900/50">
        <div className="container mx-auto px-4">
          <div className="max-w-4xl mx-auto">
            <div className="text-center mb-16">
              <h2 className="text-4xl font-bold mb-4">Cómo Jugar</h2>
              <p className="text-xl text-slate-400">Comienza en solo unos pocos pasos simples</p>
            </div>
            <div className="grid md:grid-cols-3 gap-8">
              <div className="text-center">
                <div className="w-16 h-16 bg-red-600 rounded-full flex items-center justify-center mx-auto mb-4 text-2xl font-bold">
                  1
                </div>
                <h3 className="text-xl font-semibold mb-2">Iniciar Servidor</h3>
                <p className="text-slate-400">
                  Lanza el servidor del juego para hospedar partidas y gestionar sesiones de juego.
                </p>
              </div>
              <div className="text-center">
                <div className="w-16 h-16 bg-blue-600 rounded-full flex items-center justify-center mx-auto mb-4 text-2xl font-bold">
                  2
                </div>
                <h3 className="text-xl font-semibold mb-2">Conectar Clientes</h3>
                <p className="text-slate-400">
                  Los jugadores se conectan al servidor usando la aplicación cliente del juego.
                </p>
              </div>
              <div className="text-center">
                <div className="w-16 h-16 bg-green-600 rounded-full flex items-center justify-center mx-auto mb-4 text-2xl font-bold">
                  3
                </div>
                <h3 className="text-xl font-semibold mb-2">Jugar y Competir</h3>
                <p className="text-slate-400">¡Crea partidas, editá mapas y participa en combate táctico 2D!</p>
              </div>
            </div>
          </div>
        </div>
      </section>

      {/* Download Section */}
      <section id="download" className="py-20">
        <div className="container mx-auto px-4">
          <div className="max-w-4xl mx-auto text-center">
            <h2 className="text-4xl font-bold mb-4">¿Listo para Jugar?</h2>
            <p className="text-xl text-slate-400 mb-8">
              Descarga CS2D Strike y experimenta combate táctico 2D con tus amigos
            </p>
            <div className="flex flex-col sm:flex-row gap-4 justify-center">
              <Button className="bg-red-600 hover:bg-red-700 text-lg px-8 py-6"
                sx={{
                  backgroundColor: '#dc2626',
                  color: 'white',
                  textTransform: 'none',
                  fontSize: '1.1rem',
                  borderRadius: '12px',
                  padding: '10px 20px',
                  fontWeight: 'bold',
                  '&:hover': {
                    backgroundColor: '#b91c1c'
                  }
                }}
                onClick={handleDownload}
              >
                <Download className="h-5 w-5 mr-2" />
                Descargar Juego
              </Button>
              <Button
                className="border-slate-600 text-slate-300 hover:bg-slate-800 text-lg px-8 py-6"
                sx={{
                  backgroundColor: '#ffffff',
                  color: 'black',
                  textTransform: 'none',
                  fontSize: '1.1rem',
                  borderRadius: '12px',
                  padding: '10px 20px',
                  fontWeight: 'bold',
                  border: 'thin solid #FFF',
                  transition: '0.3s',
                  '&:hover': {
                    backgroundColor: '#1e293b',
                    color: "#FFF",
                    border: 'thin solid #FFF'
                  }
                }}
                onClick={handleGithub}
              >
                <Github className="h-5 w-5 mr-2" />
                Ver en GitHub
              </Button>
            </div>
            <p className="text-sm text-slate-500 mt-6">Gratuito y código abierto • Construido con fines educativos</p>
          </div>
        </div>
      </section>

      {/* Footer */}
      <footer className="border-t border-slate-800 py-12 bg-slate-950">
        <div className="container mx-auto px-4">
          <div className="flex flex-col md:flex-row justify-between items-center">
            <div className="flex items-center space-x-2 mb-4 md:mb-0">
              <Target className="h-6 w-6 text-red-500" />
              <span className="text-xl font-bold">CS2D Strike</span>
            </div>
            <div className="text-slate-400 text-sm">
              © 2025 CS2D Strike. Desarrollado por Sebastián Kraglievich, Morena Sandroni, Mateo Bulnes y Agustín Perez Romano
            </div>
          </div>
        </div>
      </footer>
    </div>
  )
}
