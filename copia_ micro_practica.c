/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copia_ micro_practica.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dangonz3 <dangonz3@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:29:54 by dangonz3          #+#    #+#             */
/*   Updated: 2025/02/13 20:14:34 by dangonz3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h> //STDIN_FILENO
#include <stdio.h> //perror

void	ft_error(char *str1, char *str2)
{
	while(*str1)
		write(2, str1++, 1);
	while(*str2)
		write(2, str2++, 1);
	write(2, "\n", 1);
}

int	main(int argc, char **argv, char **env)
{
	int	i;
	int	fd_tmp;
	int	fd[2];
	(void)argc;

	i = 0;
	fd_tmp = dup(STDIN_FILENO); //hace que fd_dup sea una copia de STDIN_FILENO, el estandard input no es alterado
	while (argv[i] && argv[i + 1])
	{
		argv = &argv[i + 1];
		i = 0;
		while (argv[i] && strcmp(argv[i], "|") && strcmp(argv[i], ";")) //condiciones de parada, cuando llegamos al final del input, cuando encontramos un pipe o cuando encontramos un ';', que viene a ser &&, pero le da igual si hemos logrado ejecutar los comandos anteriores
			i++;
		if (strcmp(argv[0] == "cd") == 0) //cd
		{
			if (i != 2)
				ft_error("Invalid arguments for cd", NULL);
			else if (chdir(argv[1])) //chdir cambia al directorio de la ruta que le hemos pasado, si lo consigue devuelve 0, si no lo consigue devuelve -1
				ft_error("Can`t change directory to: ", argv[1]);
			//podemos comprobar que el cambio se ha hecho con perror("chdir"); el output sera similar a chdir: No such file or directory
		}
		else if (i != 0 && (argv[i] == NULL || strcmp(argv[i], ";") == 0)) //ejecuta sin pipes
		{
			if (!fork()) //si fork() es 0 estamos en el hijo
				ft_execute(argv, i, fd_tmp, env);
			else // de lo contrario estamos en el padre
			{
				close(fd_tmp); //vamos a cerrar y abrir tmp_fd para asegurarnos de que no ha sido modificado. Lo cerramos antes del bucle de waitpid() para liberar los recursos lo antes posible
				while(waitpid(-1, NULL, 0) != -1) //el primer argumento es el pid del proceso al que tenemos que espear, -1 indica que se debe esperar a cualquier hijo, NULL es un puntero a int donde podriamos almacenar el estado de salida del hijo, el último argumento modifica el comportamiento de waitpid, si es 0 le dice que bloquee la ejecucucion del programa hasta que el hijo del primer argumento (en este caso cualquiera) termine
					continue; //continue no es necesario, pero me parece que deja claro el funcionamiento del while
				fd_tmp = dup(STDIN_FILENO);
			}
		}
		else if (i != 0 && (strcmp(argv[i], "|") == 0)) //ejecuta con pipes
		{
			pipe(fd); //creamos dos fd que se comunican entre ellos, pipe[0] lectura pipe[1] excritura 				
			if (!fork())
			{
				dup2(fd[1], STDOUT_FILENO); //REVISA ESTA VAINA
				close(fd[0]);
				/* close(fd[1]); */ //esto ya lo hace dup2
				ft_execute(argv, i, fd_tmp, env);			
			}
			else
			{
				close(fd[1]);
				close(fd_tmp);
				fd_tmp = fd[0];				
			}				
		}	
	}
	close(fd_tmp);
	
	








	
}